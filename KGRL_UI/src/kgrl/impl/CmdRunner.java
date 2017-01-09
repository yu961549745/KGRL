package kgrl.impl;

import java.io.BufferedReader;
import java.io.File;
import java.io.InputStreamReader;

public class CmdRunner {
	public void run(String cmd) throws Exception {
		Process process = Runtime.getRuntime().exec(cmd, null,
				new File("D:/__study/CodeLib/KGRL/app"));
		BufferedReader reader = new BufferedReader(new InputStreamReader(
				process.getInputStream(), "UTF8"));
		String line;
		while ((line = reader.readLine()) != null) {
			System.out.println(line);
		}
		process.waitFor();
	}
}
