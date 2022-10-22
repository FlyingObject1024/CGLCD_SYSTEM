import cv2
import numpy as np
from moviepy.editor import ImageSequenceClip

np.set_printoptions(threshold=np.inf)

filepath = "./FIRST_BOSS.gif"

gif = cv2.VideoCapture(filepath)
fps = gif.get(cv2.CAP_PROP_FPS)  # fpsは１秒あたりのコマ数

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

for t in range(len(images)):
    #print(images[t])
    print('uint16t image['+str(len(images[t]))+']['+str(len(images[t][0]))+'] = {')
    for i in range(len(images[t])):
        print('    {',end="")
        for j in range(len(images[t][i])):
            color = 0;
            color = images[t][i][j][0] >> 3;
            color |= ((images[t][i][j][1] & 0xFC) << 3);
            color |= ((images[t][i][j][0] & 0xF8) << 8);
            print('{:#06x}, '.format(color), end="")
    
        print('},')
    
    print('}')



#cv2.destroyAllWindows()
