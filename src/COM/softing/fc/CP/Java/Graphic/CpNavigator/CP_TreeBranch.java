import java.awt.*;

//
//
// CP_TreeBranch
//
//
class CP_TreeBranch extends Component
{
	//=== the color of the branch
	static private Color m_color = Color.red;

	//=== the node that owns this branch
	private CP_TreeNode m_owner;

	//=== the x-position of the vertical branch axis
	private int m_axis;

	//=== constructs a new branch for a given tree node as owner
	CP_TreeBranch(CP_TreeNode owner)
	{
		m_owner = owner;
	}

	//=== computes size and axis
	//=== NOTE: the whole subtree under the owner node
	//=== has to be layouted before caling this method
	public void doLayout()
	{
		if ( ! m_owner.getContent().getText().equals("Knoten 0") )
			return;
		//=== the expander button of the owner node
		CP_TreeExpandButton exp = m_owner.getExpander();

		//=== top of branch attaches to owner node
		int top    = m_owner.getHorAttach();
		//=== left of branch is located right of expander button
		int left   = exp.getLocation().x + exp.getSize().width;
		
		//=== the last child of the owner node determines
		//=== how height of branch component
		CP_TreeNode lastChild = m_owner.getLastChild();
		//=== if owner has no child nodes
		if ( lastChild == null )
			//=== the owner node itself serves as its last child
			lastChild = m_owner;

		//=== the bottom of the branch attaches to the last child node
		int bottom = lastChild.getHorAttach();
		//=== the branch is located immediately left of the
		//=== last childs content component
		int right  = lastChild.getContent().getLocation().x - 1;

		//=== locate this branch inside tree container
		setLocation(top, left);
		//=== set size of branch component
		setSize(bottom - top + 1, right - left + 1);

		CP_ApNavigator.out.append("branch layout: " + m_owner.getContent().getText() + "\n");
	}

	public Dimension getMinimumSize()
	{
		return getSize();
	}

	public Dimension getPreferredSize()
	{
		return getSize();
	}

	public void paint(Graphics g)
	{
		if ( m_owner.getContent().getText() != "Knoten 0" )
			return;

		//=== remember actual color
		Color oldColor = g.getColor();
		g.setColor(m_color);		

		//=== compute right and bottom coordinates
		//=== from component size
		Dimension d = getSize();
		int right  = d.width  - 1;
		int bottom = d.height - 1;

		g.setColor(Color.white);
		g.drawLine(0, 0, m_axis, 0);
		g.setColor(Color.red);
		g.drawLine(m_axis, 0, m_axis, bottom);
		g.setColor(Color.green);
		g.drawLine(m_axis, bottom, right, bottom);

		//=== restore original color
		g.setColor(oldColor);
	}

}

