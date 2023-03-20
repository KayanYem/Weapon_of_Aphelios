# Weapon_of_Aphelios

> ：这个名字好中二的样子
> 
> ：就是很中二的东西
> 而且“美而无用”

项目使用了WS2812 LED灯（X5），使用STM32芯片，按键（红、蓝）。
**实现功能：**
红色按键：最右边的灯移到最左，其余四个依次向右移一位。
蓝色按键：最右边的灯不动，右边第二个灯移到最左，其余四个依次向右移一位。

**展示**

![图1](../Weapon_of_Aphelios/5.DISPLAY/pic1.jpg)
![图2](../Weapon_of_Aphelios/5.DISPLAY/pic2.jpg)
![图3](../Weapon_of_Aphelios/5.DISPLAY/pic3.jpg)
![video](../Weapon_of_Aphelios/5.DISPLAY/video.gif)


```
230225 调试
1. 需要修改硬件电路
2. 购买的按钮与设计的不同（已修改
3. LED灯电路有误，电容应该接电源和接地（已修改
4. 考虑是否更换供电方案（没有更换

230320 完整装配
1. 3D打印壳体透光效果不太好
2. 整体装配后体积较大
3. 壳体设计考虑不足导致装配麻烦
```