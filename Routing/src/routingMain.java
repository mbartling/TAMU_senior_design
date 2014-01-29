import java.awt.Color;
import java.awt.Graphics;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

import javax.swing.JFrame;
import javax.swing.JPanel;


public class routingMain extends JPanel {

	/**
	 * @param args
	 * @throws FileNotFoundException 
	 */
	
	static Antenna ant_b;
	static Antenna ant_g1;
	static Antenna ant_g2;
	static Antenna ant_g3;
	static Antenna ant_end;
	static int pathMatrix [][];
	
	public static void main(String[] args) throws FileNotFoundException {
		// TODO Auto-generated method stub
		ArrayList<ArrayList<Double>> b = new ArrayList<ArrayList<Double>> ();
		ArrayList<ArrayList<Double>> g1 = new ArrayList<ArrayList<Double>> ();
		ArrayList<ArrayList<Double>> g2 = new ArrayList<ArrayList<Double>> ();
		ArrayList<ArrayList<Double>> g3 = new ArrayList<ArrayList<Double>> ();
		ArrayList<ArrayList<Double>> end = new ArrayList<ArrayList<Double>> ();
		
		//b = readRssiFile("Ant_b.txt");
//		g1= readRssiFile("Ant_g1.txt");
//		g2= readRssiFile("Ant_g2.txt");
//		g3= readRssiFile("Ant_g3.txt");
//		end = readRssiFile("end.txt");
		
//		ant_b = new Antenna(87,208,-10,g3);
//		ant_g1 = new Antenna(256,471,10,g1);
//		ant_g2 = new Antenna(134,71,10,g2);
//		ant_g3 = new Antenna(175,140,10,b);

//		ant_b = readRssiFile("Ant_b.txt");
//		ant_g1 = readRssiFile("Ant_g1.txt");
//		ant_g2 = readRssiFile("Ant_g2.txt");
//		ant_g3 = readRssiFile("Ant_g3.txt");
		
		ant_b = readRssiFile("Ant_g2.txt");
		ant_g1 = readRssiFile("Ant_g1.txt");
		ant_g2 = readRssiFile("Ant_b.txt");
		ant_g3 = readRssiFile("Ant_g3.txt");
		
//		ant_b = readRssiFile("Ant_bnew.txt");
//		ant_g1 = readRssiFile("Ant_g1new.txt");
//		ant_g2 = readRssiFile("Ant_g2new.txt");
//		ant_g3 = readRssiFile("Ant_g3new.txt");
		
		ant_b.setWeight(-10);
		ant_g1.setWeight(10);
		ant_g2.setWeight(10);
		ant_g3.setWeight(10);
		int start_row = 0, start_col = 0; 
		int current_row = start_row, current_col = start_col;
		ArrayList<Location> path = new ArrayList<Location>();
		pathMatrix = new int[ant_b.getRSSI().size()+1][ant_b.getRSSI().get(0).size()+1];
		Location previous1 = new Location(0,0);
		Location previous2 = new Location(0,0);
		int count = 0;
		
		int size = 51;
//		ArrayList<String> reached = new ArrayList<String>();
		boolean g1_reached = false, g2_reached = false, g3_reached = false;
		while( current_row < ant_b.getRSSI().size() - size/2 && current_col < ant_b.getRSSI().get(current_row).size()-size/2){
			double [][]total_gain = new double[size][size];
			total_gain[size/2][size/2] = Integer.MIN_VALUE;
			for(int i = -size/2; i <= size/2; i++){
				for(int j = -size/2; j <= size/2; j++){
					if(i != 0 || j !=0){
						if(current_row + i > 0 && current_col + j > 0){
//							System.out.println(total_gain[i+1][j+1]);
//							System.out.println(ant_b.getWeight() + " * " + ant_b.getRSSI().get(current_row+i).get(current_col+j));
							total_gain[i+size/2][j+size/2] += ant_b.getWeight() * ant_b.getRSSI().get(current_row+i).get(current_col+j);
//							System.out.println(total_gain[i+1][j+1]);
//							System.out.println(ant_g1.getWeight() + " * " + ant_g1.getRSSI().get(current_row+i).get(current_col+j));
							total_gain[i+size/2][j+size/2] += ant_g1.getWeight() * ant_g1.getRSSI().get(current_row+i).get(current_col+j);
//							System.out.println(total_gain[i+1][j+1]);
//							System.out.println(ant_g2.getWeight() + " * " + ant_g2.getRSSI().get(current_row+i).get(current_col+j));
							total_gain[i+size/2][j+size/2] += ant_g2.getWeight() * ant_g2.getRSSI().get(current_row+i).get(current_col+j);
//							System.out.println(total_gain[i+1][j+1]);
//							System.out.println(ant_g3.getWeight() + " * " + ant_g3.getRSSI().get(current_row+i).get(current_col+j));
							total_gain[i+size/2][j+size/2] += ant_g3.getWeight() * ant_g3.getRSSI().get(current_row+i).get(current_col+j);
//							System.out.println(total_gain[i+1][j+1]);
//							total_gain[i+1][j+1] += ant_end.getWeight() * ant_end.getRSSI().get(current_row+i).get(current_col+j);
						}
					}
				}
			}
			
			double max = Integer.MIN_VALUE;
			int max_row = 0, max_col=0;
			for (int i = -size/2; i <= size/2; i++){
				for (int j = -size/2; j <= size/2; j++){
					System.out.print(total_gain[i+size/2][j+size/2] + " ");
					if (total_gain[i+size/2][j+size/2] > max){
						max = total_gain[i+size/2][j+size/2];
						max_row = i;
						max_col = j;
					}						
				}
				System.out.println();
			}
			
			current_row += max_row;
			current_col += max_col;
			
			System.out.println("(" + current_row + ", " + current_col + ")");
			Location current = new Location(current_row, current_col);
//			if(current.equals(previous1)){

				double distance_b = current.getDistance(ant_b.getLoc());
				double distance_g1 = current.getDistance(ant_g1.getLoc());
				double distance_g2 = current.getDistance(ant_g2.getLoc());
				double distance_g3 = current.getDistance(ant_g3.getLoc());
				
				double min = Math.min(distance_g1, distance_g2);
				min = Math.min(min, distance_g3);
				
//				ant_g1.setWeight(ant_g1.getWeight()+10);
//				ant_g2.setWeight(ant_g2.getWeight()+10);
//				ant_g3.setWeight(ant_g3.getWeight()+10);
				
//				if(distance_g1 == min || g1_reached){
				
				path.add(current);
				
				if(distance_g1 < size/2*Math.sqrt(2)){
					ant_g1.setWeight(0);
					g1_reached = true;
					System.out.println("1 reached");
					path.add(ant_g1.getLoc());
					current = ant_g1.getLoc();
//					current_row = current.getRow();
//					current_col = current.getCol();
					pathMatrix[current.getRow()][current.getCol()] = 1;
				}
				else if(!g1_reached)
					ant_g1.setWeight(ant_g1.getWeight()+10);

				if(distance_g2 < size/2*Math.sqrt(2)){
					ant_g2.setWeight(0);
					g2_reached = true;
					path.add(ant_g2.getLoc());
					current = ant_g2.getLoc();
					System.out.println("2 reached");
					pathMatrix[current.getRow()][current.getCol()] = 1;
//					current_row = current.getRow();
//					current_col = current.getCol();
				}
				else if (!g2_reached)
					ant_g2.setWeight(ant_g2.getWeight()+10);

				if(distance_g3 < size/2*Math.sqrt(2)){
					ant_g3.setWeight(0);
					g3_reached = true;
					path.add(ant_g2.getLoc());
					current = ant_g3.getLoc();
					System.out.println("3 reached");
					pathMatrix[current.getRow()][current.getCol()] = 1;
//					current_row = current.getRow();
//					current_col = current.getCol();
				}
				else if (!g3_reached)
					ant_g3.setWeight(ant_g3.getWeight()+10);
				
		
				System.out.println(min);
				System.out.println("ant_g1 weight" + ant_g1.getWeight());
				System.out.println("ant_g2 weight" + ant_g2.getWeight());
				System.out.println("ant_g3 weight" + ant_g3.getWeight());
				System.out.println("ant_b weight" + ant_b.getWeight());
//				System.out.println("ant_end weight" + ant_end.getWeight());
				
//			}
			previous1 = previous2;
			previous2 = current;
//			path.add(current);
			if(current_row < 0 || current_col < 0)
				break;
			pathMatrix[current_row-1][current_col-1] = 1;
			count++;
			if(count>1000)
				break;
//			if (g1_reached && g2_reached && g3_reached){
//				ant_b.setWeight(0);
//				ant_end.setWeight(1000);
//			}
//			if(current.equals(ant_end.getLoc())){
//				System.out.println("ending location reached");
//					break;
//			}
		}
		
		
		for(int i = 0; i<pathMatrix.length; i++){
			for(int j = 0; j<pathMatrix[i].length; j++)
				if(pathMatrix[i][j] == 0)
					System.out.print(" ");
				else	
					System.out.print(".");
			System.out.println();
		}
				
		JFrame f = new JFrame();
		f.getContentPane().setBackground( Color.WHITE );
	    f.getContentPane().add(new routingMain());
	    f.setSize(pathMatrix[0].length+20, pathMatrix.length+20);
	    f.setVisible(true);
	}
	
	//public static ArrayList<ArrayList<Double>> readRssiFile(String filename) throws FileNotFoundException{
	public static Antenna readRssiFile(String filename) throws FileNotFoundException{
		
		Scanner fileIn = new Scanner(new File(filename));
		System.out.println("files read");
		
		ArrayList<ArrayList<Double>> rssi = new ArrayList<ArrayList<Double>> ();
		
		int row = fileIn.nextInt();
		int col = fileIn.nextInt();
		fileIn.nextLine();
		
		while(fileIn.hasNextLine()){
			ArrayList<Double> temp = new ArrayList<Double> ();
			String line = fileIn.nextLine();
			String[] tokens = line.split("\\s");
		    for (String token : tokens){
		    	//System.out.println(token);
		        double db = Double.parseDouble(token);
		        temp.add(Math.pow(10, db/10));
		    }
			rssi.add(temp);
		}
	
		return new Antenna(row, col, rssi);		
	}	
	
	public void paint(Graphics g){
		g.setColor(Color.RED);
		g.fillOval(ant_b.getColumn()-5, ant_b.getRow()-5, 10, 10);
		for(int i = 0; i < 4; i++)
			g.drawOval(ant_b.getColumn()-10*(int)(Math.pow(i, 2)+1), ant_b.getRow()-10*(int)(Math.pow(i, 2)+1), 20 + 20*(int)Math.pow(i, 2), 20+20*(int)Math.pow(i, 2));
		g.drawString("bad", ant_b.getColumn()-10, ant_b.getRow()-10);
		g.setColor(Color.BLUE);
		g.fillOval(ant_g1.getColumn()-5, ant_g1.getRow()-5, 10, 10);
		for(int i = 0; i < 4; i++)
			g.drawOval(ant_g1.getColumn()-10*(int)(Math.pow(i, 2)+1), ant_g1.getRow()-10*(int)(Math.pow(i, 2)+1), 20 + 20*(int)Math.pow(i, 2), 20+20*(int)Math.pow(i, 2));
		g.drawString("good1", ant_g1.getColumn()-10, ant_g1.getRow()-10);
		g.fillOval(ant_g2.getColumn()-5, ant_g2.getRow()-5, 10, 10);
		for(int i = 0; i < 4; i++)
			g.drawOval(ant_g2.getColumn()-10*(int)(Math.pow(i, 2)+1), ant_g2.getRow()-10*(int)(Math.pow(i, 2)+1), 20 + 20*(int)Math.pow(i, 2), 20+20*(int)Math.pow(i, 2));
		g.drawString("good2", ant_g2.getColumn()-10, ant_g2.getRow()-10);
		g.fillOval(ant_g3.getColumn()-5, ant_g3.getRow()-5, 10, 10);
		for(int i = 0; i < 4; i++)
			g.drawOval(ant_g3.getColumn()-10*(int)(Math.pow(i, 2)+1), ant_g3.getRow()-10*(int)(Math.pow(i, 2)+1), 20 + 20*(int)Math.pow(i, 2), 20+20*(int)Math.pow(i, 2));
		g.drawString("good3", ant_g3.getColumn()-10, ant_g3.getRow()-10);
		
		g.setColor(Color.black);
		for(int i = 0; i<pathMatrix.length; i++){
			for(int j = 0; j<pathMatrix[i].length; j++){
				if(pathMatrix[i][j] == 1)
					g.fillOval(j-1, i-1, 5, 5);
			}
				
			System.out.println();
		}
	}
}
