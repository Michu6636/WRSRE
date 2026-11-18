"""Contracts for the repository's own CI workflow.

Why this exists: a broken `.github/workflows/ci.yml` fails *silently*. GitHub
simply never schedules the jobs, so pull requests show no checks and look fine.
A typo in a YAML key, a step renamed out of existence, or the Windows SEH job
quietly becoming non-blocking would all go unnoticed.

These are text/YAML contracts about the workflow file. They do not prove the
jobs pass - only a real run does that.
"""

from pathlib import Path
import re
import unittest

try:
    import yaml
except ImportError:  # pragma: no cover - CI installs PyYAML
    yaml = None

ROOT = Path(__file__).resolve().parents[2]
WORKFLOW = ROOT / ".github" / "workflows" / "ci.yml"


def load():
    if yaml is None:
        raise unittest.SkipTest("PyYAML not installed")
    # PyYAML parses the bare key `on:` as the boolean True.
    return yaml.safe_load(WORKFLOW.read_text(encoding="utf-8"))


class CiWorkflowTests(unittest.TestCase):
    def setUp(self):
        self.workflow = load()
        self.jobs = self.workflow["jobs"]

    def test_workflow_file_is_valid_yaml(self):
        self.assertIsNotNone(self.workflow, "ci.yml did not parse")
        self.assertIn("jobs", self.workflow)

    def test_expected_jobs_exist(self):
        for job in ("test", "hygiene", "build-windows"):
            self.assertIn(job, self.jobs, "job %r is missing from ci.yml" % job)

    def test_triggers_include_pull_request_and_main(self):
        # YAML reads the bare `on:` key as True.
        triggers = self.workflow.get(True, self.workflow.get("on"))
        self.assertIsNotNone(triggers, "no trigger block")
        self.assertIn("pull_request", triggers,
                      "CI must run on pull requests or it cannot gate merges")
        push = triggers.get("push") or {}
        self.assertIn("main", push.get("branches", []),
                      "CI must run on pushes to main")

    def test_windows_job_is_not_continue_on_error(self):
        """The SEH test runs only on Windows; if the job is advisory, a
        regression there is invisible."""
        self.assertFalse(
            self.jobs["build-windows"].get("continue-on-error", False),
            "build-windows must gate merges; the Windows-only SEH test is the "
            "only coverage of CommitNative's rollback contract")

    def test_windows_job_runs_the_seh_test_and_guards_against_skipping(self):
        blob = WORKFLOW.read_text(encoding="utf-8")
        self.assertIn("test_real_native_write_faults", blob,
                      "the Windows job must invoke the real-SEH test")
        # A skip on Windows means cl was missing and nothing was measured.
        self.assertRegex(
            blob, r"grep[^\n]*skipped",
            "the Windows job must fail if the SEH test skips, otherwise a "
            "green run can mean 'measured nothing'")

    def test_linux_job_runs_suites_and_warns_on_skips(self):
        blob = WORKFLOW.read_text(encoding="utf-8")
        self.assertIn("find tests -name 'test_*.py'", blob,
                      "Linux job must discover and run the test suites")
        self.assertIn("skipped", blob,
                      "Linux job must surface suites that skipped tests")

    def test_almanac_runners_are_all_invoked(self):
        blob = WORKFLOW.read_text(encoding="utf-8")
        for runner in ("tests/almanac_button/run_host_tests.py",
                       "tests/almanac_button/run_binary_sha256_tests.py",
                       "tests/almanac_button/run_tax_policy_tests.py",
                       "tests/almanac_overlay_dev/run_host_tests.py"):
            self.assertIn(runner, blob, "%s is not run by CI" % runner)

    def test_actions_are_pinned_to_a_major_version(self):
        blob = WORKFLOW.read_text(encoding="utf-8")
        for match in re.finditer(r"uses:\s*([^\s#]+)", blob):
            spec = match.group(1)
            self.assertNotIn(":latest", spec, "avoid floating action refs")
            if spec.startswith("actions/"):
                self.assertRegex(spec, r"@v\d+$",
                                 "%s should pin a major version" % spec)

    def test_hygiene_honours_gitattributes(self):
        """The encoding check must skip files marked `-text` rather than
        hardcoding exceptions, or raw evidence dumps will fail it."""
        blob = WORKFLOW.read_text(encoding="utf-8")
        self.assertIn("check-attr", blob,
                      "encoding check must consult .gitattributes")


if __name__ == "__main__":
    unittest.main(verbosity=2)
