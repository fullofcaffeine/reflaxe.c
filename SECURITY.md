# Security policy

`reflaxe.c` processes source code, compiler metadata, native headers, build
configuration, filesystem paths, and toolchain output. Please report behavior
that could cross a trust boundary through the private path below.

## Reporting a vulnerability

Email [boss@fullofcaffeine.com](mailto:boss@fullofcaffeine.com) with the subject
`[reflaxe.c security]`. This is the current authoritative non-public reporting
path.

Do not open a GitHub issue, pull request, discussion, or Beads issue containing
vulnerability details. Do not attach live credentials, private source, personal
data, destructive payloads, or secrets from another system. If email is not
suitable for the sensitive details, send only a minimal request to establish a
different protected channel.

The repository is currently private. [GitHub private vulnerability reporting
and repository security advisories](https://docs.github.com/en/code-security/concepts/vulnerability-reporting-and-management/repository-security-advisories)
are available only for public repositories, so the project does not advertise
a non-working “Report a vulnerability” form.
When the repository is public and that feature is explicitly enabled and
verified, this policy will name the GitHub advisory path as an additional
private channel.

## Supported versions

There is no published or supported release yet.

| Version | Security support |
| --- | --- |
| `main` and M0 development snapshots | Best-effort triage only; not a supported release |
| Published releases | None |

Security fixes currently target `main`. Future release lines are supported only
when this table names them explicitly; an omitted or end-of-life line receives
no promised security updates. This policy does not turn development artifacts
or package-layout probes into supported releases.

## Security scope

Examples of in-scope reports include:

- generated C/token injection or malformed source escaping;
- path traversal, unsafe output overlap, or deletion outside compiler-owned
  generated files;
- malicious header, bindgen configuration, manifest, or project configuration
  handling;
- shell, argument, environment, build-flag, or native-tool invocation injection;
- allocator, lifetime, cleanup, bounds, integer-overflow, undefined-behavior, or
  public ABI trust-boundary failures;
- an unexpected broad `hxrt` dependency or unsafe runtime-policy bypass; and
- dependency, vendoring, release artifact, signing, or provenance compromise.

Ordinary unsupported Haxe features, performance questions, documentation
errors, and non-security compiler bugs belong in the normal contribution
workflow once they can be discussed without exposing a vulnerability. When in
doubt, report privately first.

## What to include

Provide enough information to reproduce and assess the issue safely:

- affected commit, artifact, configuration, profile, runtime policy,
  environment, architecture, and native toolchain;
- impact and the trust boundary crossed;
- minimal reproduction steps or a small sanitized fixture;
- whether the behavior is deterministic and whether public exploitation is
  known; and
- suggested mitigations or disclosure constraints, if any.

Avoid testing against systems or data you do not own or have permission to use.
The project does not currently offer a bug bounty or promise payment.

## Coordinated disclosure

The maintainer aims to acknowledge a report within three business days and
provide an initial triage response within seven business days. These are
response targets, not guaranteed fix dates.

Report access is limited to people needed to reproduce, remediate, review, and
coordinate disclosure. The maintainer and reporter should agree on disclosure
timing after impact and remediation are understood. Please do not publish
details before a fix or mitigation is available unless continued private
handling would create greater user harm.

A security fix must add the appropriate regression, sanitizer, native,
runtime-policy, ABI, and provenance evidence for the affected layer. E10.T06
owns the complete compiler/runtime/interop security review, and E10.T08 owns
signed reproducible release automation; this policy does not claim those future
gates are already implemented.
