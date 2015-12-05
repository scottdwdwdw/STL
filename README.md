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
>* vector_const_iterator<br>
>* vector_iterator<br>
>* 增加list_iterator和const_list_iterator<br>

         

3.[Array类](https://github.com/scottdwdwdw/STL_implement/tree/master/array)<br>
实现：<br>
>* Element access: 　at() , 　operator[] ,　 front() ,　back() <br>
>* Iterators:begin，　cbegin, 　end , 　cend  <br>
>* Capacity: empty(),　size(), 　max_size()<br>
>* operation: fill(),　swap()<br>
>* non-member function：　operator==,　　operator!=,　operator<,　operator<=,　operator>,　　operator>=,　　get<>()<br>

4.[Algorithm](https://github.com/scottdwdwdw/STL_implement/blob/master/Algorithm/Algorithm.h)<br>
实现:
<br>
>* swap(),　iter_swap(),　fill(),　fill_n(),　equal(),　lexicographical_compare()<br>
　　uninitialized_copy(),　uninitialized_fill(), 　uninitialized_fill_n(),　

5.[Vector](https://github.com/scottdwdwdw/STL_implement/blob/master/vector/Vector.h)<br>
实现：<br>
>* 　(constructor),　(destructor),　operator=(赋值构造),　Element access,　Iterators,　　Capacity,　Non-member functions

6.[List](https://github.com/scottdwdwdw/STL/blob/master/List/List.h)<br>
实现：<br>
>*　(constructor)
>*　(destructor)
>*　Element access
>*　Iterators
>*　Capacity
>*　Modifiers
