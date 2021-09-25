# %%相关包的导入
import numpy as np
import cv2
import time
import pandas as pd
from keras.models import load_model
model = load_model('.bestfaces.h5')	# 模型导入

# %% 串口通信实现
import serial 					# 导入模块
portx="COM4"					# 端口指定
bps=115200						# 波特率指定
ser=serial.Serial(portx,bps)	# 打开串口，并得到串口对象
# %%人脸识别分类器调用
classfier = cv2.CascadeClassifier("C:/Users/milli/AppData/Local/"
								  "Programs/Python/Python39/Lib/site-packages/cv2/data/"
								  "haarcascade_frontalface_default.xml")
# %%打开摄像头
cap = cv2.VideoCapture(0)  # 使用摄像头
# %%获取实时人脸并调用分类模型分类
while True:
	ret,frame = cap.read()	# 读取当前摄像头捕获画面
	gray = cv2.cvtColor(frame,cv2.COLOR_RGB2GRAY)	# 图像灰度化
	faceRects = classfier.detectMultiScale(gray, scaleFactor=1.2, minNeighbors=3, minSize=(32, 32))	# 人脸检测
	if len(faceRects) > 0:  			# 如检测到人脸
		for faceRect in faceRects:  	# 单独框出每一张人脸
			x, y, w, h = faceRect  		# 获取人脸左上角坐标（x,y）以及人脸的宽度w和高度h
			cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 5)	# 根据人脸位置画矩形框
			Now_Face =cv2.resize(gray[y:y+h,x:x+w],(32,32))				# 获取人脸数据并进行尺寸设定
			if((h>150) and (w>150)):	# 筛选出较近的人脸进行分类
				pre_y = model.predict_classes(Now_Face.reshape(1,32,32,1).astype('float')/255)		# 调用模型分类
				time.sleep(0.005)
				print(pre_y)
				if(pre_y == 4):				# 对类别进行判定
					result=ser.write(b'o')	# 如为预设类别，对开发板发送数据
				pre_y = 5					# 初始化循环中的变量
				time.sleep(0.005)
	cv2.imshow('Face Recognition', frame)	# 显示出摄像头捕捉的实时画面
	key = cv2.waitKey(30) & 0xff			# 设定退出键
	if key ==27:  							# esc退出
		break
	time.sleep(0.001)
# %%关闭摄像头
cap.release()  				# 释放摄像头
cv2.destroyAllWindows()  	# 关闭窗口
ser.close()					#关闭串口
