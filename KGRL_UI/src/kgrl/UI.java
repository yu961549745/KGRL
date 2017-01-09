package kgrl;

import java.awt.BorderLayout;
import java.awt.Desktop;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class UI {
	private Font font = new Font("Times New Roman", Font.PLAIN, 64);
	private JFrame frame = new JFrame("KGRL");
	private JLabel dataLabel = new JLabel("Data Set :  ");
	private JComboBox<String> taskBox = new JComboBox<String>();
	private JButton trainButton = new JButton("Train");
	private JButton testButton = new JButton("Test");

	private static final UI INSTANCE = new UI();

	public static UI getInstance() {
		return INSTANCE;
	}

	private UI() {
		initUI();
		addListener();
	}

	private void initUI() {
		frame.setLayout(new BorderLayout());

		dataLabel.setFont(font);
		taskBox.setFont(font);
		trainButton.setFont(font);
		testButton.setFont(font);

		taskBox.addItem("FB_A");
		taskBox.addItem("FB_B");
		taskBox.addItem("WN_A");
		taskBox.addItem("WN_B");
		taskBox.setSelectedIndex(0);

		JPanel buttonPanel = new JPanel();
		buttonPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
		buttonPanel.add(dataLabel);
		buttonPanel.add(taskBox);
		buttonPanel.add(trainButton);
		buttonPanel.add(testButton);
		frame.add(buttonPanel, BorderLayout.CENTER);

		frame.pack();
		frame.setVisible(true);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	private ButtonListener listener = new ButtonListener();

	private void addListener() {
		trainButton.addActionListener(listener);
		testButton.addActionListener(listener);
	}

	private class ButtonListener implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {
			JButton b = (JButton) e.getSource();
			String cmd = taskBox.getSelectedItem() + "_" + b.getText() + ".bat";
			try {
				Desktop.getDesktop().open(new File(cmd));
			} catch (Exception e1) {
				e1.printStackTrace();
			}
		}

	}

}
