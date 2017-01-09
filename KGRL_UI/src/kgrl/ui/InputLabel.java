package kgrl.ui;

import java.awt.FlowLayout;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class InputLabel extends JPanel {
	private static final long serialVersionUID = -2538713649512172020L;
	private JLabel textLabel;
	private JTextField textField;

	public InputLabel(String text, int inputLength) {
		super();
		textLabel = new JLabel(text);
		textField = new JTextField(inputLength);
		this.setLayout(new FlowLayout(FlowLayout.LEFT));
		this.add(textLabel);
		this.add(textField);
	}
}
