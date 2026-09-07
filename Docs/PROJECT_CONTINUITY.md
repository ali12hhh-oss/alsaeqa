# ALSAEQA — Project Continuity / Canonical State

This file is the persistent handoff record for continuing ALSAEQA across chat sessions.

## Canonical repository

- Repository: `ali12hhh-oss/alsaeqa`
- Project: **الصاعقة / ALSAEQA**
- Engine: Unreal Engine 5 + C++/Blueprints
- Primary target: Android
- This repository is the **single canonical project**. Do not create parallel editions or restore deleted placeholder/primitives.

## Story canon

- The hero belongs to the Thunder Giants bloodline; the name describes the family/bloodline and does not mean physically giant people.
- The family once ruled realms/kingdoms in medieval centuries and possessed extraordinary thunder abilities.
- An evil force wanted control of the world/resources and attacked the Thunder Kingdom.
- The hero was about 4–5 during the attack. His father and brother were killed; his mother hid him behind a chest/box. His mother and surviving siblings were captured.
- The young hero escaped the attackers, then was captured by a **different** group of evil people/slavers.
- He was enslaved and forced to work in mines/quarries. He begins in a believable poor physical state: dirt, wounds/scars and ragged clothing, but not exaggeratedly muscular or extremely thin.
- A mine collapse sends him into a deep hole where he finds a relic/object connected to his lineage. A distinctive body mark and the relic awaken inherited thunder skills.
- His first awakened skill is primarily a survival/traversal ability, enabling him to escape the hole and the mine.
- He does not initially know his true lineage or remember his real name. The canonical hero name is **الصاعقة / Alsaeqa**, recovered gradually through memories.
- Main playable-era hero age: 22.
- Memory, appearance, clothing, armor, scars, weapons and abilities evolve gradually across many stages.

## Opening canon

Opening cinematic is short and fragmentary rather than a complete exposition, approximately 30–45 seconds:

1. 0:00–0:06 — family/rule memory
2. 0:06–0:13 — attack, deaths, hiding
3. 0:13–0:20 — escape/capture
4. 0:20–0:27 — adult slavery/mine work
5. 0:27–0:37 — collapse/relic
6. 0:37–0:45 — awakening/escape

## Adventure structure

- 70 connected stages are defined by the progression registry.
- Stage transitions are automatic, not manually selected by the player.
- Stage 1 is the return to the mines: rescue workers and defeat the slaver force.
- The adventure must remain one continuous connected journey with short polished transitions between stages.

## Current committed systems

- Progression stage types and a 1–70 stage registry.
- Progression validation and saved stage handling.
- Automatic stage-flow component with delayed transition.
- Stage objective component with progress, completion and automatic stage finalization.
- Stage 1 canonical objective defaults:
  - `RescueWorkers` = 1
  - `DefeatSlavers` = 1
- Cinematic director action moments for combat finisher and high-charge thunder impact.
- Hero progression/stage-flow/objective component integration.
- Real-asset release/import pipeline documentation and validation; primitive fallback is forbidden.

## Known unfinished/next engineering work

1. Fix and verify the hero input binding for `Mount`: it must call a `void` input wrapper (`HandleMountInput`) rather than bind the `bool MountOrDismount()` function directly.
2. Connect actual slaver death events to `DefeatSlavers` objective progress.
3. Connect actual prisoner/worker rescue events to `RescueWorkers` objective progress.
4. Add the cinematic presentation hook for automatic stage transitions.
5. Build stage-specific objective configuration beyond Stage 1 without hardcoding arbitrary counts.
6. Complete animation-driven melee hit windows instead of applying damage directly on input.
7. Implement real climb/ledge traversal and animation/IK-based fall rescue.
8. Complete wind/environment reactions for foliage, cloth, hair and sand.
9. Correct unfinished ThunderBeast abilities, especially `LightningCrossing`, `ThunderSense`, and `StormCharge` behavior.
10. Complete companion AI, rescue state, detailed persistence and save-game integration.
11. Complete dialogue/quest/event orchestration and Sequencer-driven story events.
12. Import and match the user's real final assets from the release/source packs into the Unreal content pipeline; never substitute primitive placeholders.
13. Continue Android/build validation when a real UE 5.8-capable build environment becomes available. Do not claim a successful UE build without an actual build result.

## Continuation rule

At the beginning of a new chat, inspect this file plus the latest repository commits before making changes. Continue from the existing canonical state; do not recreate systems that are already committed. After each meaningful implementation block, update this file with the new committed state and remaining blockers.

## Quality bar

ALSAEQA is intended to be a professional cinematic adventure with its own visual identity, exciting events, distinctive combat and traversal, dramatic camera work, slow-motion action moments, rescues, meaningful progression and future extensibility. Do not reduce the project to placeholder mechanics merely to make it appear complete.
