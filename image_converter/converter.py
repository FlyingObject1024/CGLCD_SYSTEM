import os
import cv2
import numpy as np
from moviepy.editor import ImageSequenceClip
from tkinter import filedialog

np.set_printoptions(threshold=np.inf)

filepath = filedialog.askopenfilename(
    title = "画像ファイルを開く",
    filetypes = [("gif file", ".gif")], # ファイルフィルタ
    initialdir = "./" # 自分自身のディレクトリ
    )

gif = cv2.VideoCapture(filepath)
fps = gif.get(cv2.CAP_PROP_FPS)  # fpsは１秒あたりのコマ数

write_filename = os.path.splitext(os.path.basename(filepath))[0] + '.txt'


images = []
i = 0
while True:
    is_success, img = gif.read()
    if not is_success:
        break

    images.append(img)
    i += 1

#cv2.namedWindow('test', cv2.WINDOW_AUTOSIZE)

print("frame_num: "+ str(len(images)))


f = open(write_filename, 'w')

if(len(images) > 1):
    f.write('const uint16_t '+ os.path.splitext(os.path.basename(filepath))[0] +'['+str(len(images))+']['+str(len(images[0]))+']['+str(len(images[0][0]))+'] PROGMEM = {\n')
else:
    f.write('const uint16_t '+ os.path.splitext(os.path.basename(filepath))[0] +'['+str(len(images[0]))+']['+str(len(images[0][0]))+'] PROGMEM = {\n')

for t in range(len(images)):

    if(len(images) > 1):
        f.write('{\n')
        
    for i in range(len(images[t])):
        f.write('    {')
        for j in range(len(images[t][i])):
            color = 0;
            color = images[t][i][j][0] >> 3;
            color |= ((images[t][i][j][1] & 0xFC) << 3);
            color |= ((images[t][i][j][2] & 0xF8) << 8);
            f.write('{:#06x}, '.format(color))
    
        f.write('},\n')

    if(len(images) > 1):
        f.write('},\n')

f.write('};\n')

f.close()


