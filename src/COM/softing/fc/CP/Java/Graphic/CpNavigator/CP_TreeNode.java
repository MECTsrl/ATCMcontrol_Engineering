import java.awt.*;

import java.util.*;
//
//
// CP_TreeNode
//
//
class CP_TreeNode 
{
	static public final CP_TreeNode NOPARENT = null;

	//=== each belongs to a tree container
	private CP_Tree            m_owner;
	//=== each node has a parent node
	private CP_TreeNode        m_parent;
	//=== each node has a set of child nodes
	private Vector			   m_childs;	 

	//=== an expand button is part of each node
	private CP_TreeExpandButton m_expander;
	//=== a branch line that connects this node 
	//=== with its children
	private CP_TreeBranch       m_branch;
	//=== the content of the node ( this is the graphic representation of the data )
	private CP_TreeNodeContent  m_content;

	private boolean m_isRoot = false;

	protected CP_TreeNode()
	{
		m_parent   = null;
		m_content  = null;
		m_expander = null;
		m_branch   = null;
		m_childs   = new Vector();
		m_isRoot   = true;

	}
	
	//=== to construct a new node you must know
	//=== the tree container to which it belongs,
	//=== the parent node and the content
	protected CP_TreeNode(
		CP_TreeNode parent,
		CP_TreeNodeData data
		)
	{
		m_parent   = parent;

		m_content  = new CP_TreeNodeContent(data);
		m_expander = new CP_TreeExpandButton(this, 11, 11);
		m_branch   = new CP_TreeBranch(this);

		m_childs  = new Vector();
	}

	//=== to construct a new node you must know
	//=== the tree container to which it belongs,
	//=== the parent node and the content
	protected CP_TreeNode(
		CP_TreeNode parent,
		CP_ConfiData data
		)
	{
		m_parent   = parent;

		m_content  = new CP_TreeNodeContent(data);
		m_expander = new CP_TreeExpandButton(this, 11, 11);
		m_branch   = new CP_TreeBranch(this);

		m_childs  = new Vector();
	}

	public boolean isRoot() 
	{
		return m_isRoot;
	}

	public void setOwner(CP_Tree tree)
	{
		m_owner = tree;
		int numChilds = countChilds();
		for(int i = 0; i < numChilds; i++)
			getChild(i).setOwner(tree);
	}
	
	//=== computes the horizontal attachment,
	//=== this is the absolute y coordinate where the branch line
	//=== hits the node
	public int getHorAttach()
	{
		int conLocY   = m_content.getLocation().y;
		int conHorAtt = m_content.getHorAttach();

		return conLocY + conHorAtt;
	}
	
	//=== computes the vertical attachment,
	//=== this is the absolute x coordinate where the branch line
	//=== hits the node
	public int getVerAttach()
	{
		int conLocX   = m_content.getLocation().x;
		int conVerAtt = m_content.getHorAttach();

		return conLocX + conVerAtt;
	}

	//=== get the tree container that owns this node
	public CP_Tree getOwner()
	{
		return m_owner;
	}

	public CP_TreeNode getParent()
	{
		return m_parent;
	}

	//=== get the content part of this node
	public CP_TreeNodeContent getContent()
	{
		return m_content;
	}
	
	//=== get the expander part of this node
	public CP_TreeExpandButton getExpander()
	{
		return m_expander;
	}
	
	//=== test if this node is expanded
	public boolean isExpanded()
	{
		return m_expander.isExpanded();
	}

	protected void removeSubtree()
	{
		m_owner.remove(m_content);
		m_owner.remove(m_expander);

		int noOfChilds = countChilds();
		for(int i = 0; i < noOfChilds; i++)
		{
			getChild(i).removeSubtree();
		}
	}

	//=== layouting of nodes and its parts
	//=== x:    do layout relative to this x coordinate
	//=== y:    do layout relative to this y coordinate
	//=== vGap: gap between two nodes
	//=== hGap: gap between expander and content
	//=== return: new y coordinate for neighbour nodes
	protected int doLayout(int x, int y, int vGap, int hGap)
	{
		//=== get sizes of node parts
		int expWidth  = m_expander.getSize().width;
		int expHeight = m_expander.getSize().height;
		int conHeight = m_content.getSize().height;
		int conHorAtt = m_content.getHorAttach();

		//=== íf expander is higher than content ...
		if ( expHeight >= conHeight )
			//=== locate expander top left
			m_expander.setLocation(x, y);
		else
		{
			//=== ... otherwise align expander 
			//=== at the center of content height
			int yOffset = (int)((conHeight - expHeight)/2);
			m_expander.setLocation(x, y + yOffset);
		}

		//=== without children expander must be invisible
		if ( countChilds() > 0 )
			m_owner.add(m_expander);

		//=== advance to x position of content
		x += expWidth + hGap;

		//=== locate content
		m_content.setLocation(x, y);
		m_owner.add(m_content);

		//=== advance coordinates
		x += conHorAtt;
		y += conHeight + vGap;

		//=== do for every child
		int noOfChilds = countChilds();
		for(int i = 0; i < noOfChilds; i++)
		{
			CP_TreeNode child = getChild(i);

			//=== if actual node is expanded
			if ( isExpanded() )
				//=== then layout its child
				y = child.doLayout(x, y, vGap, hGap);
			else
				//=== otherwise remove child and its subtree
				//=== from display
				child.removeSubtree();
		}
	
		//=== now all subtree nodes are located
		//=== and the layout of the branch lines
		//=== can be performed
		m_branch.doLayout();
		m_owner.add(m_branch);

		//=== return the new y coordinate for neighbour nodes
		return y;
	}

	public int countChilds()
	{
		return m_childs.size();
	}

	public CP_TreeNode getChild(int i)
	{
		return (CP_TreeNode)(m_childs.elementAt(i));
	}

	public CP_TreeNode getLastChild()
	{
		int noOfChilds = countChilds();
		if ( noOfChilds > 0 )
			return getChild(noOfChilds - 1);
		else
			return null;
	}

	public CP_TreeNode addChild(CP_TreeNodeData data)
	{
		CP_TreeNode n = new CP_TreeNode(this, data);
		m_childs.addElement(n);
		n.setOwner(m_owner);
		return n;
	}

	public CP_TreeNode addChild(CP_ConfiData data)
	{
		CP_TreeNode n = new CP_TreeNode(this, data);
		m_childs.addElement(n);
		n.setOwner(m_owner);
		return n;
	}

	public void setDataRecursive(CP_ConfiData data)
	{
		if ( ! isRoot() )
			m_content = new CP_TreeNodeContent(data);
		for(int i = 0; i < data.countChilds(); i++)
			addChild(data.getChild(i));
	}

}

