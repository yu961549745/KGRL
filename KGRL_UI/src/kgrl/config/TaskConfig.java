package kgrl.config;

import java.lang.reflect.Field;

public class TaskConfig {
	private int eDim;
	private int rDim;
	private double margin;
	private int batchSize;
	private int errSize;
	private int validSize;
	private String data;
	private String train;
	private String test;

	public int geteDim() {
		return eDim;
	}

	public void seteDim(int eDim) {
		this.eDim = eDim;
	}

	public int getrDim() {
		return rDim;
	}

	public void setrDim(int rDim) {
		this.rDim = rDim;
	}

	public double getMargin() {
		return margin;
	}

	public void setMargin(double margin) {
		this.margin = margin;
	}

	public int getBatchSize() {
		return batchSize;
	}

	public void setBatchSize(int batchSize) {
		this.batchSize = batchSize;
	}

	public int getErrSize() {
		return errSize;
	}

	public void setErrSize(int errSize) {
		this.errSize = errSize;
	}

	public String getData() {
		return data;
	}

	public void setData(String data) {
		this.data = data;
	}

	public int getValidSize() {
		return validSize;
	}

	public void setValidSize(int validSize) {
		this.validSize = validSize;
	}

	public String getTrain() {
		return train;
	}

	public void setTrain(String train) {
		this.train = train;
	}

	public String getTest() {
		return test;
	}

	public void setTest(String test) {
		this.test = test;
	}

	public String getTrainString() throws Exception {
		StringBuffer buffer = new StringBuffer();
		buffer.append("KGRL ");
		Field[] fields = this.getClass().getDeclaredFields();
		for (Field field : fields) {
			String name = field.getName();
			if (name.equals("train")) {
				buffer.append(" -task " + field.get(this));
			} else if (name.equals("test")) {
			} else {
				buffer.append(" -" + name + " " + field.get(this));
			}
		}
		return buffer.toString();
	}

	public String getTestString() throws Exception {
		StringBuffer buffer = new StringBuffer();
		buffer.append("KGRL ");
		Field[] fields = this.getClass().getDeclaredFields();
		for (Field field : fields) {
			String name = field.getName();
			if (name.equals("train")) {
			} else if (name.equals("test")) {
				buffer.append(" -task " + field.get(this));
			} else {
				buffer.append(" -" + name + " " + field.get(this));
			}
		}
		return buffer.toString();
	}

}
