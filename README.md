# mother
a face telephone book for mother

2015/9/18：已确认存在如下BUG，但是短期暂不修复，BUG描述如下：程序安装至携带内存卡的android手机中，选择联系人头像将导致程序异常，目前已确认引起原因是头像文件存在内存卡引发路径问题，若仅是想测试程序可用性请暂时使用不安装内存卡的android手机进行测试  

Bin:存放android安装文件  
Classes:对应cocos2d-x-3.6创建项目Classes文件夹  
Resources:对应cocos2d-x-3.6创建项目Resources文件夹  
src/org/cocos2dx/cpp:对应cocos2d-x-3.6创建项目src/org/cocos2dx/cpp文件夹  

若是想运行源码，Classes、Resources、src/org/cocos2dx/cpp覆盖本地项目即可  

你也可以浏览这里看我完成此程序的过程中遇到解决的问题（创建项目-开发项目-调试项目-发布项目）  
http://blog.csdn.net/panshiqu/article/details/45971461  

这个小Demo是写给我妈妈的，只是想让她也能用上智能手机，下面解说一下本程序的功能  
初始化会进入配置页，首先会看到绿色两个编辑框，姓名及联系方式，灰色的按键选择联系人头像，三个都输入选择好以后就可以点击右边的保存按钮，保存之后下面会新增一条联系人记录，整个页面向左滑动，也会看到新增的联系人头像，点击头像拨打电话，接下来回到配置页解释新增的两个图标，黄色铅笔是修改，一切的修改将以联系方式为准，就是说你修改联系方式我就认为你在新创建联系人信息，红色叉号则是删除联系人，最下面还有一个蓝色的退出按钮。  
