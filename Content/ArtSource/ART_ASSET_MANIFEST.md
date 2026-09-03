# ALSAEQA Real Art Asset Manifest

Status key: `SOURCE_SELECTED` means the licensed source has been verified and is ready for local import. `NOT_IN_REPO` means the binary archive is intentionally not mirrored here.

| Asset | Role | License | Status | Source |
|---|---|---|---|---|
| Quaternius Universal Base Characters | Hero / Companion / humanoid NPC base | CC0 | SOURCE_SELECTED / NOT_IN_REPO | https://quaternius.itch.io/universal-base-characters |
| Quaternius Universal Animation Library | Locomotion / combat / reactions | CC0 | SOURCE_SELECTED / NOT_IN_REPO | https://quaternius.itch.io/universal-animation-library |
| Quaternius Universal Animation Library 2 | Combat combos / parkour / movement | CC0 | SOURCE_SELECTED / NOT_IN_REPO | https://quaternius.itch.io/universal-animation-library-2 |
| Quaternius Fantasy Props MegaKit | Weapons / props / medieval dressing | CC0 | SOURCE_SELECTED / NOT_IN_REPO | https://quaternius.itch.io/fantasy-props-megakit |
| Quaternius Medieval Village MegaKit | Village / abandoned city / structures | CC0 | SOURCE_SELECTED / NOT_IN_REPO | https://quaternius.itch.io/medieval-village-megakit |
| Quaternius Stylized Nature MegaKit | Forest / vegetation / rocks | CC0 | SOURCE_SELECTED / NOT_IN_REPO | https://quaternius.itch.io/stylized-nature-megakit |

## Import acceptance criteria

An asset is marked `IMPORTED` only after it exists under `Content/Art/` in the Unreal project and has been opened/validated in Unreal Engine 5.8.

An asset is marked `INTEGRATED` only after:
- the mesh is assigned to the intended actor/Blueprint;
- materials/textures load without missing references;
- collision is valid;
- animation/skeleton retargeting is valid where applicable;
- gameplay remains independent of the visual asset;
- the real asset replaces the procedural fallback in the intended game path.

No placeholder sphere/cube is allowed to be recorded as `IMPORTED` or `INTEGRATED`.
