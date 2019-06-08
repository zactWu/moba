# Glory
2019 Tongji SSE C++ Final Project 
## 小组成员

- wxt
- jzh
- yl
- fc

-----------

### 项目选题

- 王者荣耀（2d）

-----------

### 项目进度
- [x] 环境配置
- [x] 初步入门
- [x] 尝试编写小游戏
- [x] 讨论架构
- [x] 完成demo
- [x] 完善功能
- [x] 代码重构
- [x] 网络对战功能搭建
- [x] 游戏UI美化
- [ ] 优化游戏整体体验

-----------

### 基础功能
- [x] 实现服务端:⽀持联机对战、电脑AI加入
- [x] 1v1地图
- [x] 支持至少三种英雄类型
- [x] 支持三种基础小兵
- [x] 支持金钱的各种获取方式
- [x] 支持经验、等级系统
- [x] ⽀持装备系统
- [x] 支持战绩、装备⻚面查看 
- [x] 支持⼩兵、防御塔AI 
- [x] 英雄操作⽅式可以参考英雄联盟orDota
-----------

### 进阶功能
- [ ] 5v5地图
- [ ] ⽀持更多的英雄类型
- [ ] ⽀持三种野怪 
- [x]  支持聊天
-----------

### c++特性使用情况（！！姜大佬看这里！！补全它！！）
- 类型推断 ( auto / decltype )
- Lambda 表达式
- STL 容器器
- 类和多态

-----------

# 项目规范

## 代码风格
### 1:花括号用
    if(a==b){
	   somecode;
	   somecode;
	}
	的模式；
	else{
		不要问为什么这个是第一条;
	}
### 2：禁止使用TAB，一律用空格

### 3：变量：
	①：变量本身的命名使用i_character_hp,d_character_speed这种方式
			char用c代替，int用i代替，double用d代替，str用s代替，map用m代替，vector用v代替，数组前面加上a，比如ai_hp;
			只能使用bool，int，char，double，str，STL类，弃用longlong，float，char[]，short，signed
	②：类名使用Object这种驼峰命名法来命名，尽可能的控制在一个单词以内,（暂定可以使用下划线表示继承关系)
	③：类的变量用类名前三个字母表示，如:Object obj_tree,
    ④：#define RIGHT 0
			使用到的常数一律定义成这个样子，方便修改与看清定义
	⑤（类之间的比较可以重载为第一个参数之间的比较，暂定）
### 4：函数
	①：命名采用英文，不能用拼音，使用UnitMove(RIGHT,100)这样的方式这命名
	②：尽可能使用模板，重载，提供更大的兼容性，同时使用assert检查变量类型是否符合预期
	③：尽可能用bool代替void，返回错误时return 1；
	④：尽可能使用类函数，如:
	Object::Damage(T1 t1_deep,
	               T2 t2_damage_type,
	               bool (*pf_b_attack_buff)(void)){
	               assert deep>=0;
				   assert type(t1_deep)!=char
	code;
	code;
	if(pf_b_attack_buff())return;
	code;
	}
	pf代表functionpointer

### 5：语句使用：
	①：if(a==b||
	       c==d&&
		   d==s){
    /*
	不能使用tab
	对于段落的解释使用这个
    如果if里面条件判断超出4个语句，那么用一个inline bool来替代
	*/
	somecode;//假装这里有行代码
	somecode;//假装这行注释解释了这行代码
	somecode;//注意这里顶行写会好看点，和判断的条件能岔开
	}
	②：if(DEBUG)printf("this time %d and %d\n",i,j)
	    
		当if不加花括号的时候，代码和if写在一行
		当ifelse超过三个的时候，使用switch

	③：while里面即使没东西也给加个ture
	④：代码长度不能太长，时常添加注释，时常添加assert

### 6：注释建议：
	①：文件前面加上使用的变量和函数，该文件功能
	②：函数使用前加上注释
	③：没有每行代码都加的必要，不过每个段落建议加上

## Github与Sourcetree使用

- 建议增加功能时开一个分支，完成新功能的开发时再合并

- 算了只要有提交注释就行，中文就中文吧（泣）

- 使用sourcetree进行版本控制
