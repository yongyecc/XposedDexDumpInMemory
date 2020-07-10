package cn.yongye.xposedhookcmethod;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.XposedBridge;
import de.robv.android.xposed.XposedHelpers;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

public class XposedMain implements IXposedHookLoadPackage {
    @Override
    public void handleLoadPackage(XC_LoadPackage.LoadPackageParam loadPackageParam) throws Throwable {
        /**
         * native hook 的第一步：向目标进程植入so文件
         *
         * 1：在指定包名的进程中注入InlineHook的so库
         */

        XposedBridge.log("[*] HOOK Package=" + loadPackageParam.packageName);
        XposedHelpers.callStaticMethod(Runtime.class, "nativeLoad", "/data/local/tmp/test.so", loadPackageParam.classLoader, "/system/lib");
    }
}
