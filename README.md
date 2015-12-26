# Quadrotor
迷你四旋翼

## steps
1. 用nano板接电机，接MOFSET管，接3.7v电压，PWM测试
    1. 测试用MOFSET管（把面包线焊到三个引脚上）√ 3.7v下转动 1.13A N沟道
    2. 编写一路测试PWM（程序硬编码一个特定的百分比值）√
    3. 电池测试 √
2. 测试多路PWM和空心杯驱动（与下一步画电路板同时进行）
    1. 4路PWM驱动电机测试
    2. 电池驱动测试
    3. 蓝牙通讯（制定协议√，电脑或者手机端蓝牙传输通讯）
    4. 蓝牙协议，avr端实现 √
    5. avr端实现优化
    6. 蓝牙协议，android端实现
3. 画电路板（测试板，开发板）
    1. 初步电路，万用板搭建实验
    2. 购买芯片（估计要买很多，以供下面的步骤使用）
    3. 焊接芯片
    4. 调试 -> 改正
4. 四旋翼结构设计
    1. 程序模拟（这个不太了解怎么模拟）
    2. 做一个模型进行测试
5. 画正式四旋翼电路板
    1. 焊接芯片
    2. 调试 -> 改正

## 蓝牙协议
接收bytes序列
序列格式

```
| pkg byte | pkg byte2 (optional) | ... | data len byte ( use 7 bits ) | data len byte 2 (optional) | ... | actual data | xor check sum |
```

```
pkgs
HELLO 0x48
SET_MOTOR 0x53
FEEDBACK 0x41
ERROR 0x45 
```

```
ERROR types
buffer_overflow 0x1
```

### SET_MOTOR 设置电机转速
设置电机1的转速为128
| 0x53 | 0x02 | 0x00 | 0x80 | 0xd1 |

### HELLO 包
| 0x48 | 0x0 | 0x48 |

### OK 回执
| 0x41 | 0x2 | 0x6f | 0x6b | 0x47 |

