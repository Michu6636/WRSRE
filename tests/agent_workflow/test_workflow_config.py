"""Static contracts for the WRSRE agent workflow.

These tests check repository configuration and documentation only. They do not
prove that Kilo is installed, that a model obeyed a prompt, or that native code
or the game is correct.
"""

from pathlib import Path
import json
import re
import unittest

ROOT = Path(__file__).resolve().parents[2]
CONFIG = ROOT / ".kilo" / "kilo.jsonc"
AGENTS = ("autonomous-dev", "implementation-worker", "reviewer-focus1", "reviewer-focus2")


def load_config():
    # The tracked project config intentionally remains valid JSON (a JSONC subset).
    return json.loads(CONFIG.read_text(encoding="utf-8"))


def flatten_rules(permission):
    rules = []
    for key, value in permission.items():
        if isinstance(value, str):
            rules.append((key, "*", value))
        else:
            for pattern, action in value.items():
                rules.append((key, pattern, action))
    return rules


def match(text, pattern):
    # Kilo's documented simple wildcard semantics: * any chars, ? one char.
    regex = "".join(".*" if c == "*" else "." if c == "?" else re.escape(c) for c in pattern)
    return re.fullmatch(regex, text) is not None


def evaluate(permission, tool, target="*"):
    result = "ask"
    for rule_tool, pattern, action in flatten_rules(permission):
        if match(tool, rule_tool) and match(target, pattern):
            result = action
    return result


class WorkflowConfigTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.config = load_config()
        cls.agents = cls.config["agent"]

    def test_expected_profiles_and_no_repository_step_or_model_cap(self):
        self.assertEqual(set(self.agents), set(AGENTS))
        self.assertEqual(self.agents["autonomous-dev"]["mode"], "primary")
        self.assertEqual(self.agents["implementation-worker"]["mode"], "all")
        self.assertTrue(self.agents["implementation-worker"]["hidden"])
        for name in ("reviewer-focus1", "reviewer-focus2"):
            self.assertEqual(self.agents[name]["mode"], "subagent")
            self.assertTrue(self.agents[name]["hidden"])
        for name, agent in self.agents.items():
            with self.subTest(agent=name):
                self.assertNotIn("steps", agent)
                self.assertNotIn("model", agent)
        self.assertNotIn("model", self.config)
        self.assertNotIn("subagent_model", self.config)
        self.assertTrue(self.config["compaction"]["auto"])

    def test_prompt_references_resolve_to_short_tracked_bodies(self):
        for name in AGENTS:
            prompt = self.agents[name]["prompt"]
            with self.subTest(agent=name):
                self.assertRegex(prompt, r"^\{file:\./prompts/[a-z0-9-]+\.md\}$")
                path = CONFIG.parent / prompt[len("{file:"):-1]
                self.assertTrue(path.is_file(), str(path))
                text = path.read_text(encoding="utf-8")
                self.assertTrue(text.strip())
                self.assertLessEqual(len(text), 2600)
                self.assertNotIn("TODO", text)

    def test_reviewers_are_deny_by_default_read_only(self):
        denied = ("edit", "bash", "task", "external_directory", "webfetch", "websearch",
                  "agent_manager", "goal", "goal_report", "question", "skill", "lsp",
                  "todowrite", "doom_loop")
        for name in ("reviewer-focus1", "reviewer-focus2"):
            permission = self.agents[name]["permission"]
            with self.subTest(agent=name):
                self.assertEqual(next(iter(permission)), "*")
                self.assertEqual(permission["*"], "deny")
                for tool in denied:
                    self.assertEqual(evaluate(permission, tool, "docs/engine/README.md"), "deny")
                self.assertEqual(evaluate(permission, "read", "docs/engine/README.md"), "allow")
                self.assertEqual(evaluate(permission, "grep", "pattern"), "allow")
                self.assertEqual(evaluate(permission, "read", ".env"), "deny")

    def test_lead_can_delegate_only_to_expected_children_and_use_goals(self):
        permission = self.agents["autonomous-dev"]["permission"]
        for child in ("explore", "implementation-worker", "reviewer-focus1", "reviewer-focus2"):
            self.assertEqual(evaluate(permission, "task", child), "allow")
        self.assertEqual(evaluate(permission, "task", "general"), "deny")
        self.assertEqual(evaluate(permission, "goal", "start"), "allow")
        self.assertEqual(evaluate(permission, "goal_report", "complete"), "allow")
        self.assertEqual(evaluate(permission, "agent_manager", "local"), "deny")
        self.assertEqual(evaluate(permission, "doom_loop", "*"), "ask")

    def test_git_and_kilo_mutation_guards_cover_global_options(self):
        denied = (
            "git push origin HEAD", "git -C repo push --force-with-lease",
            "git commit -m x", "git -c user.name=x commit -m x",
            "git reset --hard", "git clean -fd", "git checkout main",
            "git switch main", "git restore .", "git rebase main",
            "git merge topic", "git cherry-pick abc", "git revert abc",
            "git stash", "git worktree add ../x", "git branch evil",
            "git branch -D topic", "git tag v1", "git config user.name x",
            "git remote add origin x", "git am patch", "git update-ref HEAD abc",
            "kilo run --auto x", "kilo auth login", "kilo agent create",
            "kilo config set", "kilo plugin install x", "kilo upgrade",
        )
        allowed = (
            "git status --short", "git -C repo diff", "git log -1",
            "git show HEAD", "git ls-files", "git rev-parse HEAD", "git blame file",
            "git branch --show-current", "git -C repo branch --list",
            "git remote -v", "python -m unittest discover -s tests/agent_workflow",
        )
        for name in ("autonomous-dev", "implementation-worker"):
            permission = self.agents[name]["permission"]
            for command in denied:
                with self.subTest(agent=name, command=command):
                    self.assertEqual(evaluate(permission, "bash", command), "deny")
            for command in allowed:
                with self.subTest(agent=name, command=command):
                    self.assertEqual(evaluate(permission, "bash", command), "allow")
        worker = self.agents["implementation-worker"]["permission"]
        for command in ("scratch\\ghidra_run.bat DecompAt.java", "analyzeHeadless x", "cmd /c build.bat"):
            self.assertEqual(evaluate(worker, "bash", command), "deny")
        self.assertEqual(evaluate(self.agents["autonomous-dev"]["permission"], "bash", "cmd /c build.bat"), "allow")

    def test_config_and_instruction_edits_are_protected(self):
        protected = (".kilo/kilo.jsonc", ".kilo/prompts/autonomous-dev.md",
                     "kilo.jsonc", "opencode.json", "AGENTS.md", "docs/AGENTS.md", ".env")
        for name in ("autonomous-dev", "implementation-worker"):
            permission = self.agents[name]["permission"]
            for path in protected:
                with self.subTest(agent=name, path=path):
                    self.assertEqual(evaluate(permission, "edit", path), "deny")
            self.assertEqual(evaluate(permission, "edit", "plugins/sim_clock/sim_clock.cpp"), "allow")


class WorkflowDocumentationTests(unittest.TestCase):
    def test_canonical_contracts_cover_the_five_objectives(self):
        execution = (ROOT / "docs/workflow/EXECUTION.md").read_text(encoding="utf-8")
        review = (ROOT / "docs/workflow/REVIEW.md").read_text(encoding="utf-8")
        for phrase in ("Investigate", "Plan", "Implement", "Build/Test", "Review", "Repair",
                       "Finding Ledger", "no repository", "steps", "STATIC REVIEW",
                       "BUILD/TEST EVIDENCE", "RUNTIME VALIDATION"):
            self.assertIn(phrase, execution)
        for phrase in ("INVESTIGATION", "PLAN", "IMPLEMENTATION", "read-only",
                       "Focus 1", "Focus 2", "missing required artifact"):
            self.assertIn(phrase, review)
        self.assertNotRegex(execution + review, r"bounded repair limit|2[–-]4 rounds|stated repair limit")

    def test_task_template_has_durable_ledger_and_resume_fields(self):
        text = (ROOT / "docs/tasks/TEMPLATE.md").read_text(encoding="utf-8")
        for heading in ("## Contract", "## Prior findings imported", "## Gate status",
                        "## Finding Ledger", "## Native claim evidence", "## Build/test evidence",
                        "## Review rounds", "## Separate validation statuses",
                        "## Blocker / resume checkpoint", "## Final delivery"):
            self.assertIn(heading, text)
        self.assertIn("docs/tasks/active/YYYY-MM-DD-<slug>.md", text)

    def test_copilot_has_two_read_only_focus_reviewers(self):
        folder = ROOT / ".github/agents"
        worker = (folder / "wrsre-worker.agent.md").read_text(encoding="utf-8")
        self.assertIn("wrsre-reviewer-focus1", worker)
        self.assertIn("wrsre-reviewer-focus2", worker)
        for name in ("wrsre-reviewer-focus1", "wrsre-reviewer-focus2"):
            text = (folder / f"{name}.agent.md").read_text(encoding="utf-8")
            tools = re.search(r'^tools:\s*\[(.*)\]\s*$', text, re.M)
            self.assertIsNotNone(tools)
            self.assertEqual(set(re.findall(r'"([^"]+)"', tools.group(1))), {"read", "search"})
            normalized = " ".join(text.split())
            self.assertIn("STATIC REVIEW", normalized)
            self.assertIn("RUNTIME VALIDATION", normalized)

    def test_root_entry_is_lean_and_lazy(self):
        text = (ROOT / "AGENTS.md").read_text(encoding="utf-8")
        self.assertLessEqual(len(text), 3500)
        self.assertIn("docs/workflow/EXECUTION.md", text)
        self.assertIn("docs/workflow/REVIEW.md", text)
        self.assertIn("Load only what the task needs", text)
        self.assertNotIn("Read INSTRUCTIONS.md and the relevant parts", text)


if __name__ == "__main__":
    unittest.main()
