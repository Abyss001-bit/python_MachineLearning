# %%
import cv2                      # 图像处理包
import os                       # 目录提取包
import numpy as np              # 矩阵计算包
class_number = 5                # 设定类别数量
# %%2.数据预处理
files = os.listdir("./iimg")  # 读取目录中的文件名列表
# 数据预处理：新建空白数据集与标签集
data = np.zeros((len(files),32,32),dtype=int)   # 二维数组，存放所有图片数据
label = np.zeros((len(files),1),dtype=int)      # 一维数组，存放所有图片对应标签
# 数据预处理：数据集导入与标签的截取与添加
for i in range(len(files)):                             # 获取每一个文件中的所有数据
    data[i][:][:] = cv2.imread("./iimg/"+files[i],0)	# 输入灰度图
    label[i,0] = files[i][0]                            # 标签导入label
# %%用卷积神经网络实现人脸匹配
import matplotlib.pyplot as plt
from keras.models import Sequential
from keras.layers import Dense,Conv2D,MaxPooling2D,Flatten      # 导入每一个layer
from keras import losses
from keras import optimizers
from sklearn.model_selection import train_test_split
from keras.utils.np_utils import to_categorical                 # one_hot编码
from keras import losses

def load_datasets():
	
	x_train, x_test, y_train, y_test = train_test_split(data, label, test_size=0.2)
	# 图像的数据集一般有：张量、宽、高和通道数
	x_train = x_train.reshape(x_train.shape[0],32,32,1).astype('float')/255
	x_test = x_test.reshape(x_test.shape[0],32,32,1).astype('float')/255
	# 因为涉及到类别，所以还要记住进行One-hot编码。
	y_train = to_categorical(y_train,class_number)   # 将类别进行one-hot编码转换，类别为10
    # 绘制部分随机数据图片
	plt.subplot(3,3,1)
	plt.imshow(x_train[0])
	plt.subplot(3,3,2)
	plt.imshow(x_train[1])
	plt.subplot(3,3,3)
	plt.imshow(x_train[2])
	plt.subplot(3,3,4)
	plt.imshow(x_train[3])
	plt.subplot(3,3,5)
	plt.imshow(x_train[4])
	plt.subplot(3,3,6)
	plt.imshow(x_train[5])
	plt.subplot(3,3,7)
	plt.imshow(x_train[6])
	plt.subplot(3,3,8)
	plt.imshow(x_train[7])
	plt.subplot(3,3,9)
	plt.imshow(x_train[8])
	plt.show()
	return x_train, x_test, y_train, y_test

def model_create():
    model = Sequential()        # 创建模型
    model.add(Conv2D(filters=64,kernel_size=(5,5),input_shape=(32,32,1),activation='relu'))
    model.add(MaxPooling2D())   # 两个卷积池化层
    model.add(Conv2D(filters=128,kernel_size=(5,5),activation='relu'))
    model.add(MaxPooling2D())
    model.add(Flatten())        # 特征一维化
    model.add(Dense(units=1024, activation='relu'))     # 输出数量，激活函数
    model.add(Dense(units=128, activation='relu'))      # 加一层隐藏层，会自动将上一层的输出作为这一层的输入，输出更少
    model.add(Dense(units=class_number, activation='softmax'))  # 加一层隐藏层，多分类激活，会自动将上一层的输出作为这一层的输入，这些要自己写的都是超参数
    adam = optimizers.Adam(0.02)   # 学习率设置，用于优化时的梯度下降
    model.compile(loss=losses.CategoricalCrossentropy(), optimizer=adam)   # 设置损失函数为于多分类配套的交叉熵，优化方法为adam
    return model

def model_train(model, x_train, y_train):
    print("Train................")
    model.fit(x_train, y_train,batch_size=600,epochs=40,verbose=1)          #训练数据和标签，训练一轮中的数据个数，训练轮数，把每一轮的信息详细描述


def model_test(model, x_test, y_test):
    print("Test................")
    loss = model.evaluate(x_test, to_categorical(y_test, class_number))     # 计算损失值（注意对测试集的标签进行独热编码）
    print("loss: ", loss)                                                   # 打印损失值


def model_pre(model, test_x):
    print("Predict..................")
    pre_y = model.predict_classes(test_x)                                   # 进行预测
    return pre_y

if __name__ == "__main__":

    # 获取数据集
    x_train, x_test, y_train, y_test = load_datasets()

    # 创建神经网络的模型
    model = model_create(x_train)

    # 训练
    model_train(model, x_train, y_train)

    # 验证
    model_test(model, x_test, y_test)

    # 保存模型
    model.save('.prefaces.h5')

    # 预测
    pre_y = model_pre(model, x_test)
    y_test = np.reshape(y_test,(1,-1))
    print("真实值: {}, 预测值： {}".format(y_test, pre_y))
#%%