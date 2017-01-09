package kgrl.ui;

import java.io.IOException;
import java.io.OutputStream;

public class LogStream extends OutputStream {
	private static OutputStream instance = null;

	public static OutputStream getInstance() {
		if (instance == null) {
			instance = new LogStream();
		}
		return instance;
	}

	private static final int LENGTH = 1024;
	private byte[] buffer = new byte[LENGTH];
	private int pos = 0;
	private AppendableUI ui = AppendableUIFactory.getInstance();

	@Override
	public void write(int b) throws IOException {
		buffer[pos++] = (byte) b;
		if (pos >= LENGTH) {
			flush();
		}
	}

	@Override
	public void flush() throws IOException {
		String s = new String(buffer, 0, pos);
		ui.append(s);
		pos = 0;
	}
}
