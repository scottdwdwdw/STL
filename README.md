#STL_implement

编译环境：vs2015 <br>
参考资料：[C++ reference](http://en.cppreference.com/w/)<br>
          \<\<STL源码剖析\>\><br>

=======================
1.allocator的实现<br>
[allocator](https://github.com/scottdwdwdw/STL_implement/tree/master/allocator)<br>
实现的部分：<br>
>* 第一级配置器<br>
>* 第二级配置器<br>
>* construct<br>
>* destroy<br>
>* destroy未完成接受两个迭代器版本，特例化版本<br>

2.iterator的实现<br>
[iterator](https://github.com/scottdwdwdw/STL_implement/tree/master/iterator)<br>
实现部分：<br>
>* iterator类<br>
>* iterator Trait<br>
>* Iterator operations(advance,distance,next,prev)(通过内置型指针测试，未通过标准库容器迭代器测试)<br>
>* Range access(begin,cbegin,end,cend)(通过内置型指针测试，未通过标准库容器迭代器测试(未通过原因是使用的类型标签不在同一个命名空间中，标准库的在std::空间中))<br>
>* 增加array_iterator:<br>
          array_const_iterator(未完成->运算符)<br>
          array_iterator(未完成->运算符)<br>
          reserve_const_iterator(未完成)<br>
          reserve_iterator(未完成)<br>

3.Array类<br>
实现：<br>
>* Element access: at()  operator[]  front() back() <br>
>* Iterators:begin   cbegin end  cend  <br>
>* Capacity: empty(),size() max_size()


