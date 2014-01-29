import java.util.ArrayList;


public class Antenna {
	public int row, column;
	public Location loc;
	public double weight;
	public ArrayList<ArrayList<Double>> rssi;
	
	public Antenna(int r, int c, ArrayList<ArrayList<Double>> array_2d){
		row = r;
		column = c;
		weight = 0;
		rssi = array_2d;
		loc = new Location(r,c);
	}
	
	public Antenna(int r, int c, double w, ArrayList<ArrayList<Double>> array_2d){
		row = r;
		column = c;
		weight = w;
		rssi = array_2d;
		loc = new Location(r,c);
	}
	
	public int getRow(){
		return row;
	}
	
	public int getColumn(){
		return column;
	}
	
	public Location getLoc(){
		return loc;
	}
	
	public double getWeight(){
		return weight;
	}
	
	public ArrayList<ArrayList<Double>> getRSSI(){
		return rssi;
	}
	
	public void setWeight(double w){
		weight = w;
	}
	
		
}
