#!/usr/bin/env python3
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

FRAME_WIDTH = 128
FRAME_HEIGHT = 48
FONT_SIZE = 13
PROJECT_ROOT = Path(__file__).resolve().parent
OUTPUT_PATH = PROJECT_ROOT / "assets" / "cat_guard.png"
FONT_PATHS = (
    Path("/usr/share/fonts/noto/NotoSans-Bold.ttf"),
    Path("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"),
    Path("/System/Library/Fonts/Supplemental/Arial Bold.ttf"),
    Path("C:/Windows/Fonts/arialbd.ttf"),
)

OUTLINE = "#35282b"
FUR = "#f3dfbd"
SEAL = "#5a4142"
SEAL_DARK = "#382b30"
EYE_BLUE = "#78d8f4"
NOSE = "#c98283"


def load_font():
    for font_path in FONT_PATHS:
        if font_path.is_file():
            return ImageFont.truetype(font_path, FONT_SIZE)
    raise FileNotFoundError("No supported bold font was found")


def draw_ears(draw, offset):
    draw.polygon(
        [(offset + 10, 17), (offset + 14, 6), (offset + 23, 15)],
        fill=SEAL,
        outline=OUTLINE,
    )
    draw.polygon(
        [(offset + 28, 15), (offset + 37, 6), (offset + 41, 17)],
        fill=SEAL,
        outline=OUTLINE,
    )


def draw_eyes(draw, offset, sleeping):
    if sleeping:
        draw.arc((offset + 17, 21, offset + 24, 28), 10, 170, fill="#bdefff", width=2)
        draw.arc((offset + 28, 21, offset + 35, 28), 10, 170, fill="#bdefff", width=2)
        return

    draw.ellipse((offset + 18, 21, offset + 23, 28), fill=EYE_BLUE, outline=SEAL_DARK)
    draw.ellipse((offset + 29, 21, offset + 34, 28), fill=EYE_BLUE, outline=SEAL_DARK)
    draw.line((offset + 20, 23, offset + 20, 27), fill=SEAL_DARK)
    draw.line((offset + 31, 23, offset + 31, 27), fill=SEAL_DARK)


def draw_siamese(draw, offset, sleeping):
    draw_ears(draw, offset)
    draw.rounded_rectangle(
        (offset + 10, 12, offset + 41, 40),
        11,
        fill=FUR,
        outline=OUTLINE,
        width=2,
    )
    draw.polygon(
        [
            (offset + 25, 15),
            (offset + 35, 21),
            (offset + 34, 31),
            (offset + 26, 37),
            (offset + 17, 31),
            (offset + 16, 21),
        ],
        fill=SEAL,
    )
    draw_eyes(draw, offset, sleeping)
    draw.polygon(
        [(offset + 23, 29), (offset + 28, 29), (offset + 25, 33)],
        fill=NOSE,
        outline=SEAL_DARK,
    )
    draw.arc((offset + 20, 30, offset + 26, 36), 5, 85, fill=SEAL_DARK)
    draw.arc((offset + 25, 30, offset + 31, 36), 95, 175, fill=SEAL_DARK)


def draw_panel(draw, font, offset, active):
    background = "#c97991" if active else "#e7c384"
    border = "#f7cbd7" if active else "#fff0bd"
    text_color = "#fff8f0" if active else OUTLINE
    draw.rounded_rectangle(
        (offset + 1, 1, offset + FRAME_WIDTH - 2, FRAME_HEIGHT - 2),
        12,
        fill=background,
        outline=border,
        width=2,
    )
    draw_siamese(draw, offset, active)
    label = "PAWS OFF!" if active else "CAT GUARD"
    draw.text((offset + 47, 16), label, font=font, fill=text_color)


def main():
    image = Image.new("RGBA", (FRAME_WIDTH * 2, FRAME_HEIGHT), (0, 0, 0, 0))
    draw = ImageDraw.Draw(image)
    font = load_font()
    draw_panel(draw, font, 0, False)
    draw_panel(draw, font, FRAME_WIDTH, True)
    image.save(OUTPUT_PATH)


if __name__ == "__main__":
    main()
