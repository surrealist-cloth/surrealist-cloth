from pathlib import Path
import moviepy.video.io.ImageSequenceClip

DATA_DIR = Path(__file__).parent.parent / 'data'
IMAGE_FOLDER = DATA_DIR / 'render'
fps = 60

images = list(IMAGE_FOLDER.glob('*.png'))
images.sort(key=lambda x: int(x.stem))  # sort by frame number
images = [str(p) for p in images]

print(f'Found {len(images)} frames')
print(f'Exporting at {fps} fps')
clip = moviepy.video.io.ImageSequenceClip.ImageSequenceClip(images, fps=fps)
clip.write_videofile('output.mp4')
