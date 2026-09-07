# ALSAEQA — Project Continuity / Canonical State

This is the persistent handoff record for continuing ALSAEQA across chat sessions.

## Canonical repository
- `ali12hhh-oss/alsaeqa`
- Project: **الصاعقة / ALSAEQA**
- Unreal Engine 5 + C++/Blueprints
- Primary target: Android
- One continuous canonical project; no V1/V2/V3 forks; no primitive placeholders.

## Documentation sources
- `Docs/ALSAEQA_MASTER_PROJECT_BIBLE.md` — master project bible and complete stage 1–70 documentation.
- `Docs/COMPLETE_ADVENTURE_STAGE_ROADMAP_1_70.md` — original detailed 1–70 roadmap.
- `Docs/DEVELOPMENT_RULES.md` — repository and engineering rules.
- This file — persistent current-state handoff.

## Story canon
Hero is from the Thunder Giants family/bloodline, not literal giants. The family ruled medieval realms and possessed extraordinary thunder abilities. An evil force attacked the Thunder Kingdom to control the world/resources. Hero was about 4–5; father and brother were killed, mother hid him behind a chest/box, and mother/surviving siblings were captured. Hero escaped the attackers but was later captured by a **different** group of slavers. He grew up in mine/quarry forced labor, dirty/wounded/scarred and in ragged clothing with a believable athletic/average-fit body. A mine collapse led to a deep hole, relic, body-mark reaction and inherited thunder awakening. First ability is survival/traversal-oriented. He initially does not know his lineage or true name; **الصاعقة / Alsaeqa** is the canonical name recovered gradually. Hero is 22 in the main playable era. Appearance, clothing, armor, scars, weapons and powers evolve gradually.

## Opening canon
30–45 second fragmentary cinematic: family/rule memory → attack/deaths/hiding → escape/capture → adult slavery/mine work → collapse/relic → awakening/escape. Do not dump the entire family truth in the opening.

## Stage canon
The master bible contains the complete 1–70 descriptions. High-level arc boundaries:
- 1–10 Origin of the Heir
- 11–20 Powers, Weapons and Wider World
- 21–24 Preparation for Companion Family Arc
- 25–40 Companion Family Search
- 41–44 Hero Family Search Begins
- 45–55 Taming and Mount Adventure
- 56–59 ThunderBeast Awakening
- 60–70 ThunderBeast Mastery/Endgame Routes

Hard rules: connected revisitable world; automatic story-driven stage transitions; no manual next-stage selector; companion permanent from stage 5; companion family search strictly 25–40; after 40 she supports hero family search; mounts are gameplay/traversal systems; ThunderBeast abilities require correct progression/resources.

## Current committed foundation
- 1–70 stage types and registry.
- Progression validation and saved-stage handling.
- Automatic stage-flow component with delayed transition.
- Stage objective component with progress/completion/finalization.
- Stage 1 default objectives: `RescueWorkers` = 1 and `DefeatSlavers` = 1.
- Real slaver death linkage: only enemies explicitly marked `bCountsAsStageOneSlaver` report `DefeatSlavers`, and each enemy reports at most once.
- Real worker/prisoner rescue actor: `AALSAEQAWorkerPrisonerActor`; stage-1 rescues report `RescueWorkers` once per worker and fire the Rescue cinematic story beat.
- Player `Interact` action now finds the nearest forward-facing interactable within 240 units; `Config/DefaultInput.ini` maps it to `E`.
- Automatic stage transition now fires a dedicated `StageTransition` cinematic story beat after the progression/save update.
- Cinematic director exposes `HandleStoryBeat` as a Blueprint implementation point, so the transition/rescue beats can drive authored camera/Sequencer presentation rather than being data-only events.
- Cinematic director action moments for heavy combat and high-charge thunder.
- Hero integration with stage flow/objectives.
- Real-asset release/import documentation and primitive-fallback prohibition.
- Master project bible with full project and stage documentation.

## Engineering queue
1. Data-drive later stage objectives.
2. Animation-notify-driven melee hit windows.
3. Real climb/ledge traversal and animation/IK fall rescue.
4. Wind reactions for foliage/cloth/hair/sand.
5. Complete ThunderBeast Crossing, Sense and correct Storm Charge semantics.
6. Complete companion AI/combat/rescue/persistence.
7. Dialogue/quest/event orchestration and Sequencer story events.
8. Save persistence for story, companion, rescue, discoveries and mounts.
9. Import/match real final assets; never replace with primitives.
10. Android optimization/package/build verification when UE 5.8 build hardware exists.

## Stage 1 runtime chain
`WorkerPrisoner.Interact/Rescue` → `StageObjective.RegisterProgress("RescueWorkers")` + Rescue cinematic event.

`Player.Interact (E)` → nearest `AALSAEQAInteractable` in front of hero → worker rescue.

`Slaver.HandleDeath` (only `bCountsAsStageOneSlaver=true`) → `StageObjective.RegisterProgress("DefeatSlavers")`.

When both objectives are complete → `StageObjective.FinalizeStageIfReady` → `StageFlow.CompleteCurrentStage` → delayed automatic 1→2 progression → save stage 2 → dedicated `StageTransition` cinematic event → Blueprint `HandleStoryBeat(StageTransition)` → `OnAutomaticStageChanged`.

## Continuity protocol
At the beginning of a new chat: read this file, `Docs/ALSAEQA_MASTER_PROJECT_BIBLE.md`, `Docs/DEVELOPMENT_RULES.md`, the original stage roadmap, and latest relevant commits/files. Continue from the existing canonical state. Do not recreate completed systems. After every meaningful implementation block, update this file and commit.

## Quality gate
A feature is not done merely because a class/document exists. It must be connected to the runtime path, safe under lifecycle/failure conditions, considered for persistence and Android scalability, and documented. Never claim UE build success without an actual build result.