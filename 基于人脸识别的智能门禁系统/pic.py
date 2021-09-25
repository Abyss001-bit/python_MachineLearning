# %%相关包的导入
import cv2
import time
# %%人脸识别分类器调用
classfier = cv2.CascadeClassifier("C:/Users/milli/AppData/Local/Programs/"
                                  "Python/Python39/Lib/site-packages/cv2/data/"
                                  "haarcascade_frontalface_default.xml")
# %%用openCV操作摄像头
time_start = time.time()
cap = cv2.VideoCapture(0)  # 使用摄像头
i = 0                      # 初始化循环变量
# %%连续抓拍251张照片，并转为32*32尺寸的灰度图保存
while True:
    if(i>250):
        break
    ret,frame = cap.read()  # 读取当前摄像头捕获画面
    gray = cv2.cvtColor(frame,cv2.COLOR_RGB2GRAY)   # 图像灰度化
    faceRects = classfier.detectMultiScale(gray, scaleFactor=1.2, minNeighbors=3, minSize=(32, 32))
    if len(faceRects) > 0:  # 如检测到人脸
        for faceRect in faceRects:  # 单独框出每一张人脸
            x, y, w, h = faceRect   # 获取人脸左上角坐标（x,y）以及人脸的宽度w和高度h
            cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 5)    # 根据人脸位置画矩形框
            Now_Face =cv2.resize(gray[y:y+h,x:x+w],(32,32))	# 获取人脸数据并进行尺寸设定
            if((h>200) and (w>200)):# 筛选出较近的人脸图像
                i = i+1
                print(i)                                    # 打印当前拍摄张数
                cv2.imwrite('iimg/0_'+str(i)+'pic.jpg',Now_Face)	# 存人头照片
    cv2.imshow('Face Recognition', frame)   # 显示出摄像头捕捉的实时画面
    key = cv2.waitKey(30) & 0xff            # 设定退出键
    if key ==27:                            # esc退出
        break
    time_start = time.time()
    time.sleep(0.001)

cap.release()            # 释放摄像头
cv2.destroyAllWindows()  # 关闭窗口 