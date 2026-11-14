"""Documentation contracts, not proof that agents or native code are correct."""

from pathlib import Path
import re
import unittest
from urllib.parse import unquote, urlsplit

ROOT = Path(__file__).resolve().parents[2]
DOCS = (
    "AGENTS.md",
    "GEMINI.md",
    ".clinerules",
    "INSTRUCTIONS.md",
    "KILO_AGENT_WORKFLOW.md",
    "README.md",
    ".github/instructions/wrsre.instructions.md",
    ".github/agents/wrsre-worker.agent.md",
    ".github/agents/wrsre-reviewer.agent.md",
    ".github/agents/wrsre-reviewer-focus1.agent.md",
    ".github/agents/wrsre-reviewer-focus2.agent.md",
    "docs/README.md",
    "docs/AGENTS.md",
    "docs/engine/README.md",
    "docs/engine/TEMPLATE.md",
    "docs/ghidra-index/README.md",
    "docs/ghidra-exports/README.md",
    "docs/tasks/README.md",
    "docs/tasks/TEMPLATE.md",
    "docs/tasks/archive/README.md",
    "docs/workflow/README.md",
    "docs/workflow/EXECUTION.md",
    "docs/workflow/REVIEW.md",
    "docs/workflow/KILO.md",
    "docs/workflow/PROVIDERS.md",
    "docs/almanac-export-review.md",
    "tools/ghidra/README.md",
)


class AgentDocumentationTests(unittest.TestCase):
    def test_local_markdown_links_resolve(self):
        for name in DOCS:
            path = ROOT / name
            text = path.read_text(encoding="utf-8")
            for target in re.findall(r"\]\(([^)]+)\)", text):
                parsed = urlsplit(target)
                if parsed.scheme or not parsed.path:
                    continue
                destination = path.parent / unquote(parsed.path)
                with self.subTest(document=name, target=target):
                    self.assertTrue(destination.exists(), str(destination))

    def test_legacy_cline_entry_references_authoritative_rules(self):
        text = (ROOT / ".clinerules").read_text(encoding="utf-8")
        for name in ("AGENTS.md", "INSTRUCTIONS.md", "KILO_AGENT_WORKFLOW.md"):
            with self.subTest(name=name):
                self.assertIn(name, text)
                self.assertTrue((ROOT / name).is_file())
        self.assertNotIn("compile the hook but disable it by default", text)
        self.assertNotIn("You have full access to the pre-analyzed", text)

    def test_tracked_ghidra_scripts_match_readme_inventory(self):
        folder = ROOT / "tools/ghidra"
        actual = {path.name for path in folder.glob("*.java")}
        readme = (folder / "README.md").read_text(encoding="utf-8")
        listed = set(re.findall(r"^\| `([^`]+\.java)` \|", readme, re.M))
        self.assertEqual(actual, listed)

    def test_ghidra_comments_do_not_confuse_virtual_and_file_addresses(self):
        folder = ROOT / "tools/ghidra"
        for name in ("DecompAt.java", "DisasmMany.java", "FindDisp.java"):
            with self.subTest(script=name):
                text = (folder / name).read_text(encoding="utf-8")
                self.assertNotIn("Addresses are FILE-BASED", text)
                self.assertIn("not file offsets", text.lower().replace("raw file offsets", "file offsets"))


if __name__ == "__main__":
    unittest.main()
