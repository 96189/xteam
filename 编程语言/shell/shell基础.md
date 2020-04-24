## 函数

* 函数定义与传参
```
// 获取指定目录下所有文件的{filename filesize md5val}信息，每行一条
get_fileinfo() {
    dir=$1
    info=""
    for file in `ls $dir`; do
        if [ ! -f "$file" ]; then
            continue
        fi
        filesize=$(stat -c%s "$file")
        md5val=`md5sum $file | awk '{ print $1 }'`
        info=$info"$file $filesize $md5val\n"
    done
    echo -e "$info"
}
```

* 按行读文件存数组(如何动态向数组中添加值)
```
function read_file() {
    file_path=$1
    content_vec=$2
    while IFS= read -r line
    do
        conetent_vec[${#content_vec[@]}]=$line
    done < "$file_path"
}
```

* 在数组中查找，数组的应用以及接受返回值
```
function for_each_array() {
    key=$1
    value_list=$2
    exist="n"
    for item in "${value_list[@]}"
    do
        if [ "$item" == "$key" ]; then
            exist="y"
            break
        fi
    done
    return "$exist"
}

declare -a value_list=("a", "b", "c")
for_each_array "a" value_list
find_res=$?
```

