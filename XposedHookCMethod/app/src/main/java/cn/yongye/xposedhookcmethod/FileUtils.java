package cn.yongye.xposedhookcmethod;


import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;


public class FileUtils {

    public static void cpFile (String stSrcFp, String stDestFp) {
        BufferedInputStream in = null;
        BufferedOutputStream out = null;
        try {
            in = new BufferedInputStream(new FileInputStream(stSrcFp));
            out = new BufferedOutputStream(new FileOutputStream(stDestFp));

            byte[] btTmp = new byte[1024];
            int readLen = 0;
            while((readLen = in.read(btTmp)) != -1){
                out.write(btTmp, 0, readLen);
            }

            in.close();
            out.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static String getContent(String fp){
     String stContent = null;
     try {
         byte[] btTmp = new byte[1024];
         FileInputStream oIn = new FileInputStream(new File(fp));
         int inContentLen = oIn.read(btTmp);
         byte[] btContent = new byte[inContentLen];
         System.arraycopy(btTmp, 0, btContent, 0, inContentLen);
         stContent = new String(btContent);
     } catch (FileNotFoundException e) {
         e.printStackTrace();
     } catch (IOException e) {
         e.printStackTrace();
     }
        return stContent;
    }
}
