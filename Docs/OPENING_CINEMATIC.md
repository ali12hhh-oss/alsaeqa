# ALSAEQA — Professional Opening Cinematic

## Status
The project already contained the opening story seed in Stage 1 (`Chains in the Mine`) and a general cinematic-action contract. It did **not** previously contain a dedicated, shot-by-shot specification for the game's opening film. This document is now the canonical opening-cinematic blueprint.

## Goal
The first minutes must immediately establish that ALSAEQA is a cinematic third-person adventure, not a prototype. The opening should feel mysterious, emotional and dangerous, then hand control to the player without an abrupt transition.

Target runtime: **3–4 minutes**, with a shorter mobile-safe fallback edit using the same narrative beats.

## Narrative structure

### Sequence 01 — The Storm Before Memory (0:00–0:30)
- Begin in near darkness with distant wind, low thunder and stone vibration.
- Reveal a vast ancient landscape through clouds, rain and intermittent lightning.
- A colossal Thunder Giant silhouette appears only in fragments: hand, armor, ancient weapon and eyes reflected in rain.
- Do not explain the entire mythology. The audience should understand that an ancient power once existed.
- Lightning briefly reveals enormous ruins and a monumental path disappearing into the storm.

### Sequence 02 — The Fall of the Thunder Legacy (0:30–1:20)
- Move through the ancient ruins as the storm intensifies.
- Show the Thunder lineage being overwhelmed by an unseen enemy force through silhouettes, distant movement, firelight, collapsing architecture and reactions rather than graphic violence.
- A family symbol is protected during the chaos.
- A final defensive action connects the symbol to a dormant thunder source.
- The sequence ends on a violent thunder flash that wipes the image to white, then silence.

### Sequence 03 — The Heir (1:20–2:00)
- Transition from the ancient era to the present through the same thunder sound becoming a mine echo.
- Reveal the hero years later in confinement inside the mine.
- Close shots establish breathing, restrained hands, dirt, exhaustion and determination.
- The hero notices a faint ancient mark in the stone.
- A distant thunder vibration answers the mark.
- The first tiny electrical spark appears naturally, not as a UI effect.

### Sequence 04 — The Collapse (2:00–2:50)
- A structural failure begins far away.
- Dust, rocks, chains, wood and debris react physically.
- The camera alternates between controlled cinematic angles and close survival shots.
- The hero escapes restraints and runs as the mine collapses.
- Include one signature last-second survival beat: the hero falls, catches a ledge, struggles, then pulls himself up.
- The moment should establish the game's future traversal language without pretending the full traversal system is already available at this point.

### Sequence 05 — First Spark (2:50–3:25)
- The hero reaches a narrow opening overlooking the outside world.
- Wind hits clothing and dust moves through the opening.
- Clouds part briefly.
- A lightning bolt strikes an ancient object/stone structure in the distance.
- The hero instinctively raises his hand; a controlled spark answers.
- Camera pushes in slowly as the soundscape drops to a heartbeat and distant thunder.
- The hero looks toward the ancient route.

### Sequence 06 — Title and Hand-off (3:25–4:00)
- A final wide shot establishes the world and the route ahead.
- The game title **ALSAEQA / الصاعقة** appears with restrained thunder illumination, not a cheap flash.
- Music resolves into the game's main motif.
- The camera transitions into the exact gameplay camera position for Stage 1.
- HUD remains hidden until player control is confirmed.
- Player control begins at the mine exit/starting gameplay location defined by the Stage 1 level.

## Camera language
- Use deliberate cinematic composition: wide environmental reveals, low-angle power shots, intimate facial/body shots, controlled tracking and motivated camera movement.
- Avoid constant camera shake. Reserve it for structural collapse, thunder impact and major danger.
- Use depth, foreground objects, silhouettes and environmental occlusion to create scale.
- Never use a random camera move just to look cinematic; every shot must reveal information, emotion or danger.

## Character performance
- Hero: restrained fear -> confusion -> determination -> instinctive awakening.
- Ancient figures: communicate power through posture and scale rather than exposition.
- Facial expressions must use the real facial/animation assets when available.
- Clothing, hair and body motion must react to the mine wind, dust and storm pressure where the supplied assets support it.

## Audio direction
- Start with environmental sound before music.
- Layer low thunder, stone vibration, chains, breathing, distant impacts, debris and wind.
- Music enters gradually instead of starting at full volume.
- Use silence immediately before the first meaningful thunder response.
- All final audio must be licensed/owned for commercial use before shipping.

## VFX direction
- Real Niagara-based lightning/thunder effects are required for the hero's awakening and major storm beats.
- Dust, debris, sparks and atmospheric particles must respond to the event rather than appear as disconnected effects.
- The first spark is intentionally small; later powers can become spectacular as progression advances.
- Do not use primitive placeholder meshes or placeholder VFX in the final cinematic.

## Gameplay hand-off rules
- The opening is one-shot per new game.
- It must never replay unexpectedly after a checkpoint reload.
- Skip is optional and should be disabled during critical loading/initialization boundaries; if enabled, it must transition cleanly to the Stage 1 gameplay camera.
- Completion must set the opening-complete state before normal Stage 1 gameplay starts.
- If a cinematic asset is unavailable, the build must fail the cinematic validation rather than silently substituting primitive placeholder content.

## Unreal authoring contract
The C++ cinematic director exposes an opening lifecycle and a Blueprint event. The actual Level Sequence, Cine Cameras, animation tracks, facial animation, audio, Niagara systems, environment dressing and final shots must be authored in Unreal Editor using the project's real assets. This separation is intentional: code controls state/lifecycle; Sequencer controls the authored film.

## Stage 1 connection
The opening must flow directly into `Chains in the Mine`, the first stage of the continuous 1–70 adventure. Stage 1 is already defined as the escape from the opening collapse and awakening of the first spark.

## Quality bar
The opening is not considered complete until:
1. The final real hero/character assets are used.
2. The final environment assets are dressed and lit.
3. The cinematic uses authored camera shots/Sequencer rather than only gameplay camera movement.
4. Facial/body animation is authored for the key beats.
5. Thunder, wind, dust and debris are synchronized.
6. Audio/music transitions are authored.
7. The title reveal and hand-off to gameplay are seamless.
8. Android scalability has a lower-cost cinematic path without changing the story.
