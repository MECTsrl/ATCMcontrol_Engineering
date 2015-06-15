import java.util.Vector;

//
//
// CP_NodeList
//
//
class CP_NodeList 
{
	private Vector m_nodes;

	//==================================================
	/** Creates a new empty node list
	 */
	//==================================================
	CP_NodeList()
	{
		//=== vector of nodes is empty
		m_nodes = new Vector();
	}

	public int countNodes()
	{
		return m_nodes.size();
	}

	public CP_TreeNode getNodeAt(int i)
	{
		return (CP_TreeNode)(m_nodes.elementAt(i));
	}

	public void addNode(CP_TreeNode n)
	{
		m_nodes.addElement(n);
	}
}

