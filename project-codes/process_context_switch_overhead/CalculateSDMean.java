import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.math.BigInteger;
import java.nio.charset.Charset;

public class CalculateSDMean {

	public static final String RESULTS_FILENAME = "results.out";
	public static final long CPU_FREQUENCY = 2500000L;

	public static void main(String args[]) {
		try {
			createOutputFile(RESULTS_FILENAME, "start_results.out", "end_results.out");
			System.out.println("Calculating Count");
			long count = getCount(RESULTS_FILENAME);
			System.out.println("Calculating Mean");
			long mean = getMean(RESULTS_FILENAME, count);
			System.out.println("Calculating Standard Deviation");
			double sd = getSD(RESULTS_FILENAME, count, mean);
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

	public static void createOutputFile(String output_Filename, String start_filename, String end_filename)
			throws Exception {
		FileOutputStream out = new FileOutputStream(output_Filename);
		OutputStreamWriter oswr = new OutputStreamWriter(out, Charset.forName("UTF-8"));
		BufferedWriter bw = new BufferedWriter(oswr);

		FileInputStream in = new FileInputStream(start_filename);
		InputStreamReader insr = new InputStreamReader(in, Charset.forName("UTF-8"));
		BufferedReader br = new BufferedReader(insr);

		FileInputStream in2 = new FileInputStream(end_filename);
		InputStreamReader insr2 = new InputStreamReader(in2, Charset.forName("UTF-8"));
		BufferedReader br2 = new BufferedReader(insr2);

		BigInteger value = BigInteger.ZERO;
		String start_line = null, end_line = null;
		while ((start_line = br.readLine()) != null && (end_line = br2.readLine()) != null) {
			BigInteger start = new BigInteger(start_line);
			BigInteger end = new BigInteger(end_line);
			value = end.subtract(start);
			bw.write(value.toString());
			bw.write("\n");
		}
		bw.flush();
		bw.close();
		oswr.close();
		out.close();
		br.close();
		insr.close();
		in.close();
		br2.close();
		insr2.close();
		in2.close();
	}
}
