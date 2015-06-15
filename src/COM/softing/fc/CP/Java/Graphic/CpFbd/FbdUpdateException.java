public class FbdUpdateException extends Exception
{
   public FbdUpdateException(String msg)
   {
	   super("Invalid Update String: " + msg);
   }

}
