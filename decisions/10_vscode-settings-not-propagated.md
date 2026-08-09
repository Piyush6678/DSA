# 10 — `.vscode/settings.json` contents were not carried into `CLAUDE.md`

## Context

`/init` asks that existing rule files — `.cursor/rules/`, `.cursorrules`,
`.github/copilot-instructions.md` — be folded into `CLAUDE.md`. None exist here. The only
configuration found was `.vscode/settings.json`:

```json
{
  "files.associations": { "*.ejs": "html", "random": "cpp" },
  "github.copilot.enable": { "*": false, "plaintext": false, "markdown": false, "scminput": false }
}
```

## Decision

Note that the search was done and turned up nothing worth propagating; leave the file
undocumented.

## Why

Neither key affects how code should be written. `files.associations` is editor syntax
highlighting — the `"random": "cpp"` entry maps the extensionless `<random>` header, and
`"*.ejs": "html"` is left over from unrelated work. `github.copilot.enable` is a personal
preference about a different tool, not a constraint on this repo.

Copying them in would pad `CLAUDE.md` with facts that never change a decision, which is
the specific failure `/init` warns about. Recording the *absence* of rule files matters
more than the settings themselves — it tells a future session the search was already run.

## Rejected alternative

Quoting the settings under a "Configuration" heading for completeness. Rejected —
completeness is not the goal; every line in `CLAUDE.md` competes for attention with the
lines that do change behavior.
