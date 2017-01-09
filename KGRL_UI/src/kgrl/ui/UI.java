package kgrl.ui;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.FileReader;
import java.lang.reflect.Field;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

import kgrl.config.AppConfig;
import kgrl.config.TaskConfig;
import kgrl.impl.CmdRunner;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

public class UI implements AppendableUI {
	private JFrame frame = new JFrame("KGRL");
	private JLabel dataLabel = new JLabel("Data Set");
	private JComboBox<String> taskBox = new JComboBox<String>();
	private JButton trainButton = new JButton("Train");
	private JButton testButton = new JButton("Test");
	private JTextArea logArea = new JTextArea(20, 150);
	private JScrollPane scrollPane = new JScrollPane(logArea);

	private static final UI INSTANCE = new UI();

	public static UI getInstance() {
		return INSTANCE;
	}

	private UI() {
		try {
			loadConfig();
		} catch (Exception e) {
			e.printStackTrace();
		}
		initUI();
		addListener();
	}

	private AppConfig config = null;

	private void loadConfig() throws Exception {
		Gson gson = new GsonBuilder().create();
		config = gson.fromJson(new FileReader("config.json"), AppConfig.class);
	}

	private void initUI() {
		frame.setLayout(new BorderLayout());

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
		frame.add(buttonPanel, BorderLayout.NORTH);

		frame.add(scrollPane, BorderLayout.CENTER);
		logArea.setEditable(false);

		frame.pack();
		frame.setVisible(true);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	private CmdRunner runner = new CmdRunner();

	private void addListener() {
		trainButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					disable();
					String cmd = getTaskConfig().getTrainString();
					System.out.println(">> " + cmd);
					runner.run(cmd);
					enable();
				} catch (Exception e1) {
					e1.printStackTrace();
				}
			}
		});
		testButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					disable();
					String cmd = getTaskConfig().getTestString();
					System.out.println(">>" + cmd);
					runner.run(cmd);
					enable();
				} catch (Exception e1) {
					e1.printStackTrace();
				}
			}
		});
	}

	private void disable() {
		trainButton.setEnabled(false);
		testButton.setEnabled(false);
	}

	private void enable() {
		trainButton.setEnabled(true);
		testButton.setEnabled(true);
	}

	private TaskConfig getTaskConfig() throws Exception {
		String name = taskBox.getSelectedItem().toString();
		Field field = AppConfig.class.getDeclaredField(name);
		return (TaskConfig) field.get(config);
	}

	@Override
	public void append(final String str) {
		logArea.append(str);
	}
}
