package kgrl.impl;

import java.io.BufferedReader;
import java.io.InputStreamReader;

public class CmdRunner {
	public void run(String cmd) throws Exception {
		System.out.println(">> " + cmd);
		readOutput(cmd);
	}

	private void readOutput(final String cmd) {
		new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					Process process = Runtime.getRuntime().exec(cmd);
					BufferedReader reader = new BufferedReader(
							new InputStreamReader(process.getInputStream(),
									"GBK"));
					String line;
					while ((line = reader.readLine()) != null) {
						System.out.println(line);
					}
					process.waitFor();
					reader.close();
				} catch (Exception e) {
					e.printStackTrace();
				}
				System.out
						.println("----------------------END-----------------------");
			}
		}).start();
	}
}
