# ALSAEQA — Character Damage & Injury Standard

The injury system applies consistently to the hero, permanent companion, allies, workers and enemies.

## States
- Healthy
- Injured
- Critical
- KnockedOut / unconscious
- Dead

## Body regions
Head, torso, left/right arm, left/right leg. Each region has independent severity and can drive movement/combat penalties through Blueprint presentation.

## Organs
The gameplay model supports abstract internal organ states (heart, lungs, liver) for high-impact damage, medical/story events and AI reactions. These are gameplay state values, not graphic anatomical simulation.

## Dismemberment
Limb loss is an explicit state for arms and legs. The C++ system records the state and exposes Blueprint presentation hooks. Final mesh separation, sockets, VFX and camera presentation must use the project's real character assets and must avoid placeholder geometry.

## Knockout
A character can become unconscious without dying. Knockout is recoverable and is distinct from death. AI can use it for rescue, capture, interrogation, recovery and story events.

## Character-specific behavior
- Hero: injuries can affect movement/combat presentation and may trigger contextual recovery or story consequences.
- Companion: injuries can trigger downed/assist/revive behavior and temporary movement limitations.
- Allies/workers: injuries can cause panic, limping, collapse, escort and rescue states.
- Enemies: injuries can alter aggression, locomotion, attack choices, retreat and surrender/knockout behavior.

## Production rule
No graphic gore or anatomical detail is required for the gameplay layer. The system is intentionally state-driven so the same combat event can produce an appropriate presentation for the rating, platform and asset set.

## Persistence
Permanent injuries, knockout state and story-critical body-state consequences must be included in save data only when the story/gameplay design requires them. Temporary combat injuries should normally reset on safe recovery/checkpoint.
