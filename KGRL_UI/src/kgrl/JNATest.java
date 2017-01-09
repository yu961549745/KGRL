package kgrl;

import com.sun.jna.Native;
import com.sun.jna.win32.StdCallLibrary;

public interface JNATest extends StdCallLibrary {
	public static JNATest INSTANCE = (JNATest) Native.loadLibrary("DLL.dll",
			JNATest.class);

	public void runKGRL(int eDim, int rDim, double margin, int batchSize,
			int errSize, String dataName, String taskName, int validSize);
}
