# 页面置换模拟器

## 概述

模拟器模拟了一个简单的页式虚拟内存管理。已经实现的功能有：

1. 利用`trace_path`中存放的内存访问序列计算Hit/Miss和近似比。主要用于真实数据的性能测试。
2. 利用随机序列测试Hit/Miss和近似比。
3. **Hacker**：利用提供的库和类`Array`，编写访存程序并测试近似比，方便简单测例的测试。

## 编译

使用命令`make`编译。

## Trace 列表

- `short`：小的例子

- `matrix-inverse`：一个类似矩阵转置的访问序列

#### `trace`格式

- 首先一个数`len`，表示访存序列的长度；

- 接下来`len`行，每行先一个字符`R/W`，表示这个操作是读/写；接下来一个整数，表示访问的页面号。

## 算法列表

- `rp_ran`：随机置换算法（Rand）
- `rp_fifo`：先入先出算法（FIFO）
- `rp_lru`：最近最久未访问算法（LRU）
- `rp_clock`：带脏标记的时钟置换算法（Clock）
- `rp_marking`：随机标记算法（Random Marking）

## 增加算法

#### 实现算法

抽象类`page_rp`是页面置换算法的基类，其中实现了不命中时调用缺页处理程序、统计换入换出次数等基本功能。

其中你需要了解的成员变量是`map<size_t, int> mem`。

- 这个`map`记录了当前在内存中的页和其相关信息，在`mem[pos]`中存放着页面`pos`的各种标记。例如`PTE_P, PTE_A, PTE_D`。

为了实现一个新的算法，**你需要实现一个子类，并重写以下虚函数**：

- `reset_hook(int n)`：当进行清空时会调用这个函数，你需要在这里清空你使用的成员变量。
- `write_hook(size_t pos)`：当发生一次写操作时，系统会调用这个函数。你可以在这里做一些处理。
- `read_hook(size_t pos)`：当发生一次读操作时，系统会调用这个函数。你可以在这里做一些处理。
  - 请注意，在真实系统中并不会存在上面的两个调用。这两个函数仅供记录一些访问的临时信息（例如缺页的页面）或维护页面存在性，不能用来记录额外的信息或执行其他操作。
- `find_swap()`：系统发生缺页且内存已满，需要选择一个页面进行换出。返回需要换出的页面。

一些其他的使用函数如下：

- `get_mask(size_t pos)`：获取一个页面的标记，页面不存在时报错。
- `set_mask(size_t pos, int mask)`：修改页面标记，页面不存在时报错。
- `bool inside(size_t pos)`：判断一个页面是否存在于内存中

#### 测试算法

在`analyze_list.cpp`中加入你的算法。其中：

- `analyze_random(page_rp &algo)`：使用随机数据测试。随机数据模拟了系统过饱和，访存几乎随机时的情景。
- `analyze_trace(page_rp &algo, bool rand)`：使用`trace`测试，其中`rand=true`时会多次测试取平均值。

以上两个测试都固定逻辑内存为`512 Pages`，分别对物理内存为`8, 12, 16, ..., 512 Pages`的情况统计近似比。

## 增加Hacker

Hacker是一个利用简单访存程序测试算法的机制。一个简单的`hacker`如下：

```c++
class certain_hacker : public hacker {

public:

	certain_hacker()
	{ name = "certain hacker"; }
	virtual void main(Simulater *sim)
	{
		this->sim = sim;
		size_t len = pgsize * 129;
		Array arr = sim->new_array(len);
		size_t last = 0;
		for (int i = 0; i < 1 << 20; i++) {
			arr[last] = 1;
			last = (last + pgsize) % len;
		}
	}
} hacker_certain;
```

这个`hacker`循环的访问相邻的`129`个页面。当物理内存大小为`128`个页面时，LRU和FIFO就会发生大量缺页。

其中，`Array`是虚拟储存库提供的一个工具，模拟了一个数组。可以使用：

```c++
Array arr = sim->new_array(len);
```

创建一个长度为`len`的模拟数组，并使用`arr[i]`来访问数组。**请注意**：储存库并不真的完成数组的存储和计算，只会统计相关的访存信息。

#### 内存参数

模拟器模拟的内存参数在`sim_mem.cpp`中定义：

-  `pgsize`为每一页的大小。

#### 实现hacker

你需要实现一个继承自`hacker`的子类，并实现`main(Simulater *sim)`函数。`sim`传入其使用的模拟器，可以用来创建数组。

为了代码结构的清晰，你可以将其写在单独的文件中，并在`hacker_driver.cpp`中include这个文件。

#### 测试算法

系统会根据`hacker_driver/HackDriver::mem_size`来测试不同的物理内存大小。你可以修改类的构造函数来加入不同内存大小的测试。

为了测试一个算法，还要在`hack_list`中加入新的算法。

