"""Structural smoke checks for the Windows-only native translation unit.

Linux host tests compile the portable headers, not private_logistics.cpp itself.
These checks catch an unbalanced native function body before a Windows build.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / "plugins/private_logistics/private_logistics.cpp"


def without_comments_or_literals(text):
    out = []
    i = 0
    state = "code"
    quote = ""
    while i < len(text):
        c = text[i]
        n = text[i + 1] if i + 1 < len(text) else ""
        if state == "code":
            if c == "/" and n == "/":
                state = "line_comment"
                out.extend("  ")
                i += 2
                continue
            if c == "/" and n == "*":
                state = "block_comment"
                out.extend("  ")
                i += 2
                continue
            if c in ('"', "'"):
                state = "literal"
                quote = c
                out.append(" ")
                i += 1
                continue
            out.append(c)
        elif state == "line_comment":
            if c == "\n":
                state = "code"
                out.append(c)
            else:
                out.append(" ")
        elif state == "block_comment":
            if c == "*" and n == "/":
                state = "code"
                out.extend("  ")
                i += 2
                continue
            out.append("\n" if c == "\n" else " ")
        else:
            if c == "\\" and n:
                out.extend("  ")
                i += 2
                continue
            if c == quote:
                state = "code"
            out.append("\n" if c == "\n" else " ")
        i += 1
    if state in ("block_comment", "literal"):
        raise AssertionError(f"unterminated {state}")
    return "".join(out)


class NativeCppStructure(unittest.TestCase):
    def test_braces_balance_and_native_functions_are_at_file_scope(self):
        code = without_comments_or_literals(SOURCE.read_text(encoding="utf-8"))
        depth = 0
        depths = []
        for c in code:
            if c == "{":
                depth += 1
            elif c == "}":
                depth -= 1
                self.assertGreaterEqual(depth, 0, "closing brace without an opener")
            depths.append(depth)
        self.assertEqual(depth, 0, "unclosed native function or scope")
        for declaration in ("static int DetourTransfer(", "static bool PatchSite("):
            with self.subTest(declaration=declaration):
                pos = code.index(declaration)
                self.assertEqual(depths[pos], 0, f"{declaration} is nested inside another scope")


if __name__ == "__main__":
    unittest.main()
