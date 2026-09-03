# ALSAEQA — Real 3D Art Pipeline

## Goal

The final game presentation must use authored 3D meshes, skeletal characters, real animation clips, textured weapons/props, and authored environment kits. Runtime primitive geometry is only a safety fallback and is **not** the final art direction.

## Commercial-safe sources selected

All selected Quaternius packs below explicitly permit use, modification and commercial use under CC0 / the current Quaternius Asset License. The repository must not redistribute the source packs as standalone asset packs.

| Production role | Source | Use |
|---|---|---|
| Hero + companion humanoid base | Universal Base Characters | Rigged game-ready human bases, male/female, retargetable |
| Locomotion + combat | Universal Animation Library | 120+ humanoid animations, locomotion, combat, death, root-motion variants |
| Advanced combat/parkour | Universal Animation Library 2 | 130+ combat combos, parkour and movement animations |
| Fantasy weapons/props | Fantasy Props MegaKit | Swords, axes, tools, chests, furniture, environment props |
| Town/city structures | Medieval Village MegaKit | Modular walls, floors, roofs, stairs and buildings |
| Forest/rocks/vegetation | Stylized Nature MegaKit | Trees, plants, flowers, rocks, grass and bushes |

Official source pages:
- https://quaternius.com/packs/universalbasecharacters.html
- https://quaternius.com/packs/universalanimationlibrary.html
- https://quaternius.com/packs/universalanimationlibrary2.html
- https://quaternius.com/packs/fantasypropsmegakit.html
- https://quaternius.com/packs/medievalvillagemegakit.html
- https://quaternius.com/packs/stylizednaturemegakit.html
- License: https://quaternius.com/license.html

## Import layout

After downloading the official packages, import into Unreal 5.8 using this structure:

```text
Content/Art/
  Characters/
    Hero/
    Companion/
    Enemies/
    Bosses/
  Creatures/
    GiantSnake/
    Mounts/
  Weapons/
  Environment/
    Mine/
    Forest/
    ThunderMountain/
    Caves/
    MonsterValley/
    AbandonedCity/
    TyrantFortress/
    BlackDesert/
    IceKingdom/
    LandOfGiants/
  Animations/
    Locomotion/
    Combat/
    Parkour/
    Reactions/
    Death/
  Materials/
  VFX/
```

## Character production

### Hero
- Universal Base Character humanoid rig.
- Distinct hero outfit assembled from modular fantasy clothing.
- Thunder-themed material accents, not a primitive glow-only representation.
- Real sword/weapon mesh attached to the hand socket.
- Animation Blueprint: idle, walk, jog, sprint, jump, fall, land, light attack, heavy attack, hit reaction, dodge, thunder abilities, death.

### Companion
- Separate female base character and outfit.
- Different silhouette and weapon loadout from hero.
- Independent animation state machine.
- Combat and rescue montages remain separate from the hero.

### Enemies / bosses
- Use dedicated creature/humanoid meshes from the licensed source pool where available.
- Bosses receive unique silhouette, materials, weapons and attack montages rather than merely larger scale.
- Giant Snake uses a dedicated creature mesh and attack animation set; no segmented primitive fallback is considered final.

### Mounts
- Real skeletal animal/creature mesh.
- Idle, walk, run, sprint, turn, jump, land, injured, mounted reaction animations.
- ThunderBeast receives its own emissive material/VFX layer and specialized ability montages.

## Animation retargeting

The Quaternius Universal Animation Libraries are designed for retargeting and include Unreal-compatible GLB exports. Use the matching humanoid skeleton for the Universal Base Characters, then create an Unreal IK Rig / IK Retargeter pair.

Required animation groups:

1. Locomotion: idle, walk, jog, sprint, strafe, 8-direction movement.
2. Traversal: jump, fall, land, climb, crawl, swim, parkour.
3. Combat: light, heavy, combo 1–4, recovery, hit reactions, dodge, parry.
4. Weapon: sword/axe/staff-style attacks and equip/unequip where compatible.
5. State: injured, staggered, knockdown, death.
6. Companion: independent locomotion, combat, rescue/capture/rejoin states.

Root motion must be selected intentionally per montage. The newer Quaternius exports provide both root-motion and non-root-motion variants.

## Real environment production

The world remains one interconnected world. The following regions receive distinct authored kits/material treatment rather than cubes:

- Mine: modular timber supports, rails, rock walls, carts, lamps and debris.
- Forest: layered trees, undergrowth, rocks, paths, cliffs and points of interest.
- Thunder Mountains: exposed rock, ancient structures, storm VFX and high-altitude silhouettes.
- Caves: modular rock formations, crystals/ancient traces and authored collision.
- Monster Valley: large rocks, vegetation, creature nests and traversal routes.
- Abandoned City: modular buildings, streets, ruins, props and destructible-looking dressing.
- Tyrant Fortress: modular stone/wood structures, gates, towers, weapons and banners.
- Black Desert: authored rock formations, sparse vegetation, ruins and atmospheric dressing.
- Ice Kingdom: snow/ice materials, structures, frozen props and traversal hazards.
- Land of Giants: oversized architecture, rocks, ancient structures and scale landmarks.

The existing RuntimeWorldBuilder remains useful for development/testing, but its primitive/greybox output must be replaced progressively by these imported authored assets before final release.

## Weapons and props

Weapons are real meshes with sockets and authored materials. The code should select a weapon asset through a configurable `TSoftObjectPtr<UStaticMesh>` or Blueprint data asset rather than hard-coding a primitive mesh.

Props use static meshes with simple collision; animated props use skeletal/static mesh animation as appropriate.

## Hazards and VFX

Hazards are visualized with authored meshes plus Niagara where appropriate:
- falling rocks
- collapsing mine supports
- spikes/traps
- fire/lava zones
- ice hazards
- electrical storm zones
- boss arena hazards
- breakable/blocked traversal objects

Gameplay collision remains independent from presentation so the art can be replaced without changing combat/progression logic.

## Repository rule

Do **not** commit an asset merely because a website says it is downloadable. Keep a manifest containing source URL, asset name, license, date checked, and intended use. Do not redistribute paid/proprietary source archives. CC0 assets may be used in the game, but the repository should avoid turning the repository itself into an asset-pack mirror.

## Current limitation

GitHub's connected file API available to this development session accepts UTF-8 text but cannot upload binary GLB/FBX/PNG/ZIP files. Therefore this commit adds the real-art acquisition/import pipeline rather than falsely claiming binary art packs have already been uploaded. The official pages above are the selected sources; the actual binary import must occur on the Unreal development machine/runner.
