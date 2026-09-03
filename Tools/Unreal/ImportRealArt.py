"""Unreal Editor Python helper for ALSAEQA real-art import.

Run with Unreal Editor's Python plugin enabled. The script creates the final
Content/Art folder contract, scans imported assets, and reports what still
needs to be wired. It intentionally does not create fake meshes.
"""
import unreal

ROOT = "/Game/Art"
FOLDERS = [
    "Characters/Hero", "Characters/Companion", "Characters/Enemies", "Characters/Bosses",
    "Creatures/GiantSnake", "Creatures/Mounts",
    "Weapons", "Environment/Mine", "Environment/Forest", "Environment/ThunderMountain",
    "Environment/Caves", "Environment/MonsterValley", "Environment/AbandonedCity",
    "Environment/TyrantFortress", "Environment/BlackDesert", "Environment/IceKingdom",
    "Environment/LandOfGiants", "Animations/Locomotion", "Animations/Combat",
    "Animations/Parkour", "Animations/Reactions", "Animations/Death", "Materials", "VFX",
]


def ensure_folders():
    for rel in FOLDERS:
        path = f"{ROOT}/{rel}"
        if not unreal.EditorAssetLibrary.does_directory_exist(path):
            unreal.EditorAssetLibrary.make_directory(path)


def classify(asset):
    if isinstance(asset, unreal.SkeletalMesh):
        return "SkeletalMesh"
    if isinstance(asset, unreal.StaticMesh):
        return "StaticMesh"
    if isinstance(asset, unreal.AnimationSequence):
        return "AnimationSequence"
    if isinstance(asset, unreal.AnimBlueprint):
        return "AnimBlueprint"
    if isinstance(asset, unreal.MaterialInterface):
        return "Material"
    return type(asset).__name__


def audit():
    ensure_folders()
    assets = unreal.EditorAssetLibrary.list_assets(ROOT, recursive=True, include_folder=False)
    counts = {}
    for path in assets:
        obj = unreal.EditorAssetLibrary.load_asset(path)
        if obj is None:
            continue
        kind = classify(obj)
        counts[kind] = counts.get(kind, 0) + 1
    unreal.log("=== ALSAEQA REAL ART AUDIT ===")
    unreal.log(f"Root: {ROOT}")
    for kind in sorted(counts):
        unreal.log(f"{kind}: {counts[kind]}")
    if not counts:
        unreal.log_warning("No real art assets imported yet. Import approved GLB/FBX packs before wiring actors.")
    unreal.log("Primitive runtime visuals remain fallback only.")
    return counts


if __name__ == "__main__":
    audit()
