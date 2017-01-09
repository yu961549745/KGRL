package kgrl;

import javax.swing.UIManager;

public class RunKGRL {
	public static void main(String[] args) {
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {
			e.printStackTrace();
		}
		UI.getInstance();
	}
}
