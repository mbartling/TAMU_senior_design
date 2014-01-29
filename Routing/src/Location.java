
public class Location {
	int row, column;
	
	public Location(int r, int c){
		row = r;
		column = c;
	}
	
	@Override
	public String toString(){
		return "(" + row + ", " + column + ")";
	}
	
	public boolean equals(Location l){
		return row == l.row && column == l.column;
	}
	
	public int getRow(){
		return row;
	}
	
	public int getCol(){
		return column;
	}
	
	public double getDistance(Location l){
		double distance = Math.sqrt(Math.pow(row - l.getRow(), 2) + Math.pow(column - l.getCol(),2));
		return distance;	
	}
}
