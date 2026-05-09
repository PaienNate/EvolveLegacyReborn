# EvolveLegacyReborn Prerelease

This archive is a development artifact package.

What is included:

- `BINTRUST.dll`
- `EvolveLogging.ini`
- `EvolveCrack/`
- `certs/`
- `EvolveLogging.internal.example.ini`
- `THIRD_PARTY_NOTICES.txt`

What is not included:

- A patched `Evolve.exe`
- A complete launcher workflow
- A turnkey end-user release process

To test the embedded server path:

1. Set `use_internal_server = true` in `EvolveLogging.ini`.
2. Copy the `EvolveCrack/` and `certs/` directories beside the game binaries.
3. Keep the external CA bundle in sync with `certs/mitmproxy-ca-cert.pem`.

Optional internal-server keys are documented in `EvolveLogging.internal.example.ini`.
