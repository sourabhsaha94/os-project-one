import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.nio.charset.Charset;

public class CalculateSDMean {

	public static final String RESULTS_FILENAME = "results.out";
	public static final long CPU_FREQUENCY = 2500000L;

	public static void main(String args[]) {
		try {
			System.out.println("Calculating Count");
			long count = getCount("results.out");
			System.out.println("Calculating Mean");
			long mean = getMean("results.out", count);
			//double meanTime = getMicorsecondTimeFromCycles(mean);
			System.out.println("Calculating Standard Deviation");
			double sd = getSD("results.out", count, mean);
			//double sdTime = getMicorsecondTimeFromCycles(sd);
			System.out.printf("Count : %d, Mean : %d, Standard Deviation : %f\n", count, mean, sd);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static long getCount(String filename) throws Exception {
		FileInputStream in = new FileInputStream(filename);
		InputStreamReader insr = new InputStreamReader(in, Charset.forName("UTF-8"));
		BufferedReader br = new BufferedReader(insr);
		long count = 0;
		String line = null;
		while ((line = br.readLine()) != null) {
			count++;
		}
		br.close();
		insr.close();
		in.close();
		return count;
	}

	public static long getMean(String filename, long count) throws Exception {
		FileInputStream in = new FileInputStream(filename);
		InputStreamReader insr = new InputStreamReader(in, Charset.forName("UTF-8"));
		BufferedReader br = new BufferedReader(insr);
		String line = null;
		long sum = 0L;
		while ((line = br.readLine()) != null) {
			sum += Long.parseLong(line);
		}
		sum /= count;
		br.close();
		insr.close();
		in.close();
		return sum;
	}

	public static double getSD(String filename, long count, long mean) throws Exception {
		FileInputStream in = new FileInputStream(filename);
		InputStreamReader insr = new InputStreamReader(in, Charset.forName("UTF-8"));
		BufferedReader br = new BufferedReader(insr);
		String line = null;
		double sum_meansubtracted_squared = 0L;
		while ((line = br.readLine()) != null) {
			double sum = (Double.parseDouble(line) - mean);
			sum = sum * sum;
			sum = sum / count;
			sum_meansubtracted_squared += sum;
		}
		br.close();
		insr.close();
		in.close();
		return (double) Math.sqrt(sum_meansubtracted_squared);
	}

}
