##[如何编译FFmpeg 并且引入Android Studio](http://www.jianshu.com/p/c7bab9c020f0)

##如何在Android 中使用FFmpeg命令 ？

### 在FFmpeg目录在找到

cmdutils.c cmdutils.h cmdutils_common_opts.h config.h ffmpeg.c ffmpeg.h ffmpeg_filter.c ffmpeg_opt.c（注意：config.h需要编译后才有）到 cpp 目录下。

### 把FFmpeg 下面的文件也需要copy 到 cpp下

![100011.png](http://upload-images.jianshu.io/upload_images/1157085-319f574562188cd1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 修改部分文件

修改cmdutils.c 和 cmdutils.h
找到cmdutils.c中的exit_program函数
修改前:
```code
int exit_program(int ret)
{
     if (program_exit)
        program_exit(ret);
    exit(ret);
 }

```

修改后：
```code
int exit_program(int ret)
{
    if (program_exit)
        program_exit(ret);
    return ret;
}

```
找到cmdutils.h中exit_program的申明，也把返回类型修改为int：如下：

```code
int exit_program(int ret);
```
修改ffmpeg.c与ffmpeg.h
找到ffmpeg.c，把int main(int argc, char argv) 改名为 int run(int argc, char argv) （名字可以随便起）
找到ffmpeg.h, 在文件末尾添加函数申明: int run(int argc, char **argv);（需要和ffmpeg.c 所起名字保持一致）

###编写文件调用：
```code
JNIEXPORT jint JNICALL
Java_com_maiml_ffmpeglibrary_FFmpeg_run(JNIEnv *env, jobject instance, jobjectArray commands) {

    // TODO
    av_log_set_callback(my_logcat);

    int argc = env->GetArrayLength(commands);
    char *argv[argc];
    int i;
    for (i = 0; i < argc; i++) {
        jstring js = (jstring) env->GetObjectArrayElement(commands, i);
        argv[i] = (char *) env->GetStringUTFChars(js, 0);
    }
    return run_command(argc, argv);
}
```

到这里就可以运行FFmpeg命令了。一直我也是这样使用，但是我在做这个项目[Cut](http://sj.qq.com/myapp/detail.htm?apkName=com.miaml.mcut)的时候发现连续调用多次FFmpeg命令会报错（在项目需要，先改变分镜头的速度，再合成视频）。
为什么会这样的呢？
FFmpeg每次执行完命令后会调用 ffmpeg_cleanup 函数清理内存，并且会调用exit(0)结束当前进程，但是经过我们的修改，exit()的代码已经被删掉，而我们的App还在运行，不能杀死进程，所以有些变量的值还在内存中，这样就会导致下次执行的时候可能会出错。
打开ffmpeg.c找到刚修改的run函数，然后在 return 前加上如下代码即可:
```code
 nb_filtergraphs = 0;
     progress_avio = NULL;

     input_streams = NULL;
     nb_input_streams = 0;
     input_files = NULL;
     nb_input_files = 0;

     output_streams = NULL;
     nb_output_streams = 0;
     output_files = NULL;
     nb_output_files = 0;
```


参考文章
[FFmpeg](https://ffmpeg.org/documentation.html)
[编译Android下可执行命令的FFmpeg](http://blog.csdn.net/mabeijianxi/article/details/72904694)

END。
