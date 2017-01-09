package kgrl;

import java.io.PrintStream;

import javax.swing.UIManager;

import kgrl.ui.AppendableUIFactory;
import kgrl.ui.LogStream;

public class RunKGRL {
	public static void main(String[] args) {
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {
			e.printStackTrace();
		}
		AppendableUIFactory.getInstance();
		System.setOut(new PrintStream(LogStream.getInstance(), true));
		System.setErr(new PrintStream(LogStream.getInstance(), true));
	}
}
