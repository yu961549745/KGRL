package kgrl;

public class Test {
	public static void main(String[] args) throws Exception {
		// Gson gson = new GsonBuilder().create();
		// AppConfig config = gson.fromJson(new FileReader("config.json"),
		// AppConfig.class);
		//
		// CmdRunner runner = new CmdRunner();
		// runner.run("ping www.baidu.com");

		// runner.run(config.WN_B.getTestCmd());

		JNATest test = JNATest.INSTANCE;
		test.runKGRL(3, 3, 1, 100, 1, "FB_A", "LP_Train", 100);
	}
}
