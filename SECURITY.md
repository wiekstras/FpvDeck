# Security policy

FpvDeck is pre-release and has no supported production version yet. Network access
must never be required for FPV operation, and no automatic forced-update channel is
planned.

Please do not publish a vulnerability that could affect radio control, battery or
power safety, filesystem integrity, or device update trust before maintainers have
had a reasonable opportunity to assess it. Use GitHub's private vulnerability
reporting for `wiekstras/FpvDeck` when available. If that channel is unavailable,
open a minimal issue requesting a private contact without including exploit details.

For ordinary crashes, simulator faults, and non-sensitive reliability defects, use
the bug or hardware issue templates. No file in `hardware/pcb/` should be treated as
production-safe unless its revision is explicitly released as such.
