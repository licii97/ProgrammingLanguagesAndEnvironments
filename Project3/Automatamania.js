/*
 * Automatamania
 *

Student 1: 56554 Pascal Maximilian Engel
Student 2: 56601 Felicitas Schmelz

Comment:

Javascript Module to interact with loaded (json) finite automatons

01234567890123456789012345678901234567890123456789012345678901234567890123456789

INSIDE THIS FILE, YOU CAN CHANGE EVERYTHING YOU WANT!
/*

/*
DOCUMENTATION

HTML and DOM documentation:
	http://ctp.di.fct.unl.pt/miei/lap/teoricas/17.html
	https://www.w3schools.com/html/default.asp
	https://www.w3schools.com/js/js_htmldom.asp

CSS documentation:
	https://www.w3schools.com/css/default.asp

JavaScript introduction:
	http://ctp.di.fct.unl.pt/miei/lap/teoricas/18.html
	http://ctp.di.fct.unl.pt/miei/lap/teoricas/19.html
	https://www.w3schools.com/js/default.asp

Cytoscape.js documentation:
	http://js.cytoscape.org/
*/

/* UTILITY GLOBAL FUNCTIONS ------------------------------------------------- */

function equals(a, b) {  // fairly general structural comparison
	if( a === b )
		return true;
	if( !(a instanceof Object) || !(b instanceof Object) )
		return false;
	if( a.constructor !== b.constructor )
		return false;
	const aProps = Object.getOwnPropertyNames(a);
	const bProps = Object.getOwnPropertyNames(b);
	if( aProps.length != bProps.length )
		return false;
	for( var i = 0; i < aProps.length; i++ ) {
		const propName = aProps[i];
		if( !equals(a[propName], b[propName]) )
			return false;
	}
	return true;
}

function belongs(v, arr) {
	return arr.some(x => equals(x, v));
}

function canonical(arr) {
	// This version do not sort, so that we can control the order of the
	// states of the finite automata in the screen presentation
	const res = [];
	arr.forEach( x => { if(!belongs(x, res)) res.push(x); } );
	return res;
}

function canonicalPrimitive(arr) {
	// This version do not sort, so that we can control the order of the
	// states of the finite automata in the screen presentation
	return Array.from(new Set(arr));
}

function cons(v, arr) {
	return [v].concat(arr);
}

function consLast(v, arr) {
	return arr.concat(v);
}

function isEmpty(arr) {
	// Beware: arr == [] does not work
	// For example, note that [] == [] is false
	return arr.length == 0;
}

function intersection(arr1, arr2) {
	return arr1.filter(value => arr2.includes(value));
}

function addAll(v, arr) {
	return arr.map(l => cons(v,l));
}

function flatMap(f, arr) {
	return arr.map(f).reduce((acc,a)=>consLast(acc,a), []);
}

function clone(obj) {
	return Object.assign({}, obj);
}


/* FINITE AUTOMATA ---------------------------------------------------------- */

class AbstractAutomaton {  // common features to FA, PDA, TM, etc.
	constructor() {}
}

class FiniteAutomaton extends AbstractAutomaton {
	constructor(fa) {
		super();
		this.initialState = fa.initialState;
		this.transitions = fa.transitions;
		this.acceptStates = fa.acceptStates;
	}

	getStates() {
		return canonical(
			[this.initialState].
			concat(this.transitions.map(([s,_0,_1]) => s)).
			concat(this.transitions.map(([_0,_1,s]) => s)).
			concat(this.acceptStates)
		);
	}

	getAlphabet(){
		return canonical(this.transitions.map(([_0,a,_1]) => a));
	}


	gcut(s, ts) {
		return [ts.filter(([z,_0,_1]) => z == s),
				ts.filter(([z,_0,_1]) => z != s)];
	}

	reachableX(s, ts) {
		const [x, xs] = this.gcut(s,ts);
		return cons(s, flatMap(([_0,_1,z]) => this.reachableX(z,xs), x));
	}

	reachable() {
		return canonical(this.reachableX(this.initialState, this.transitions));
	}

	productive() {
		const allStates = this.getStates();
		const reachAccepted =
				s => !isEmpty(intersection(this.acceptStates,
							this.reachableX(s, this.transitions)));
		return allStates.filter(reachAccepted);
	}

	transitionsFor(s, symb) {
		return this.transitions.filter(([s1,symb1,_]) =>
									s == s1 && symb == symb1);
	}

	acceptX(s, w) {
		if( isEmpty(w) )
			return this.acceptStates.includes(s);
		else {
			const [x,...xs] = w;
			const ts = this.transitionsFor(s,x);
			if( isEmpty(ts) )
				return false;
			else {
				const [[_0,_1,s],..._] = ts;
				return this.acceptX(s,xs);
			}
		}
	  }

	accept(w) {
		return this.acceptX(this.initialState,w);
	}

	deterministic(){
		console.log(this.transitions);
		console.log(this.transitions.map(([s,t,_0]) => [s,t]));
		return (this.transitions.length ==
			canonical(this.transitions.map(([s,t,_0]) => [s,t])).length);
	}

	generateX(n,s,ts,a){
		if(n == 0){
			if(a.includes(s)) return [[]];
			else return [];
		}
		else {
			var xy = this.gcut(s,ts);
			var x = xy[0];
			return x.flatMap(([_0,symb,s]) =>
				addAll(symb,this.generateX(n-1,s,ts,a)));
		}
	}

	generate(n){
		return this.generateX(n,this.initialState,
			this.transitions,this.acceptStates);
	}
}

const abc = new FiniteAutomaton({
	initialState: "START",
	transitions: [
			["START",'a',"A"], ["START",'b',"START"],
						["START",'c',"START"], ["START",'d',"START"],
			["A",'a',"A"], ["A",'b',"AB"], ["A",'c',"START"], ["A",'d',"START"],
			["AB",'a',"A"], ["AB",'b',"START"],
						["AB",'c',"SUCCESS"], ["AB",'d',"START"],
			["SUCCESS",'a',"SUCCESS"], ["SUCCESS",'b',"SUCCESS"],
						["SUCCESS",'c',"SUCCESS"], ["SUCCESS",'d',"SUCCESS"]
		],
	acceptStates: ["SUCCESS"]
});

function testAll() {
//	getAlphabet
	console.log("");
	console.log(abc.getStates());
	console.log("");
	console.log(abc.gcut(abc.initialState, abc.transitions));
	console.log("");
	console.log(abc.reachable());
	console.log("");
	console.log(abc.productive());
	console.log("");
	console.log(abc.accept(['a','b','c']));
	console.log("");
	console.log(abc.accept(['a','b']));
//	generate
//	accept2
}

//Â testAll();



/* CYTOSCAPE GRAPHS AND USER INTERFACE -------------------------------------- */

// global constants and variables
var cyGraph;

const cyGraphStyle = {
	  layout: {
		name: 'grid',
		rows: 2,
		cols: 2
	  },

	  style: [
		{ selector: 'node[name]',
		  style: {
			'content': 'data(name)'
		  }},

		{ selector: 'edge',
		  style: {
			'curve-style': 'bezier',
			'target-arrow-shape': 'triangle',
			'label': 'data(symbol)'
		  }},

		// some style for the extension

		{ selector: '.eh-handle',
		  style: {
			'background-color': 'red',
			'width': 12,
			'height': 12,
			'shape': 'ellipse',
			'overlay-opacity': 0,
			'border-width': 12, // makes the handle easier to hit
			'border-opacity': 0
		  }},

		{ selector: '.eh-hover',
		  style: {
			'background-color': 'red'
		  }},

		{ selector: '.eh-source',
		  style: {
			'border-width': 2,
			'border-color': 'red'
		  }},

		{ selector: '.eh-target',
		  style: {
			'border-width': 2,
			'border-color': 'red'
		  }
		},

		{ selector: '.eh-preview, .eh-ghost-edge',
		  style: {
			'background-color': 'red',
			'line-color': 'red',
			'target-arrow-color': 'red',
			'source-arrow-color': 'red'
		  }},

		{ selector: '.eh-ghost-edge.eh-preview-active',
		  style: {
			'opacity': 0
		  }},

		//selector for giving reachable nodes a purple color
		{
			selector: '.reachableNode',
			style: {
				'color': 'purple'
			}
		},
		//selector for giving productive nodes a yellow color
		{
			selector: '.productiveNode',
			style: {
				'color': 'yellow'
			}
		}
	  ]
};

class CyGraph {
	constructor(nodes, edges, fa) {
		const spec = clone(cyGraphStyle);
		spec.elements = { "nodes": nodes, "edges": edges};
		spec.container = document.getElementById('cy');
		this.cy = cytoscape(spec);
		this.cy.$('#START').select();
		this.fa = fa;
	}

	static build(fa) {
		const nodes = fa.getStates().map(function(x){
			return { data: { id: x, name: x } }});
		const edges = fa.transitions.map(function(x){
			return { data: { source: x[0], symbol: x[1], target: x[2] } }} ) ;
		return new CyGraph(nodes, edges, fa);
	}

	static load(text) {
		try {
			const json = JSON.parse(text);
			const fa = new FiniteAutomaton(json);
			return this.build(fa);
		} catch( ex ) {
			alert(ex);
		}
	}

	static sampleGraph() {
		return new CyGraph(
			[
			  { data: { id: "START", name: "START" } },
			  { data: { id: "A", name: "A" } },
			  { data: { id: "AB", name: "AB" } },
			  { data: { id: "SUCCESS", name: "SUCCESS" } }
			],
			[
			  { data: { source: "START", symbol: 'a', target: "A" } },
			  { data: { source: "START", symbol: 'b', target: "START" } },
			  { data: { source: "START", symbol: 'c', target: "START" } },
			  { data: { source: "START", symbol: 'd', target: "START" } },
			  { data: { source: "A", symbol: 'a', target: "A" } },
			  { data: { source: "A", symbol: 'b', target: "AB" } },
			  { data: { source: "A", symbol: 'c', target: "START" } },
			  { data: { source: "A", symbol: 'd', target: "START" } },
			  { data: { source: "AB", symbol: 'a', target: "A" } },
			  { data: { source: "AB", symbol: 'b', target: "START" } },
			  { data: { source: "AB", symbol: 'c', target: "SUCCESS" } },
			  { data: { source: "AB", symbol: 'd', target: "START" } },
			  { data: { source: "SUCCESS", symbol: 'a', target: "SUCCESS" } },
			  { data: { source: "SUCCESS", symbol: 'b', target: "SUCCESS" } },
			  { data: { source: "SUCCESS", symbol: 'c', target: "SUCCESS" } },
			  { data: { source: "SUCCESS", symbol: 'd', target: "SUCCESS" } }
			],
			abc
		);
	}

}


/* EVENT HANDLING ----------------------------------------------------------- */

function onLoadAction(event) {
	cyGraph = CyGraph.sampleGraph();
	statistics(cyGraph);
}

const recolorNode = (
	(node) => {
		cyGraph.cy.style()
			.selector(cyGraph.cy.filter(function(element, i){
				return !element.selected() && element.isNode()
								&& element.data('id') === node;
			}))
			.style({
				'background-color': 'gray'
			})
			.update()
		;
		cyGraph.cy.style()
			.selector(cyGraph.cy.filter(function(element, i){
				return element.selected() && element.isNode()
								&& element.data('id') === node;
			}))
			.style({
				'background-color': 'blue'
			})
			.update()
		;
	}
);

function onMouseUpAction(event) {
	//reset all nodes' colors when (un-)selecting node
//cyGraph.cy.on('select', 'edge', e => cy.elements().not(e.target).unselect());
	cyGraph.cy.on('select', e => cyGraph.fa.getStates().forEach(recolorNode));
	cyGraph.cy.on('unselect', e => cyGraph.fa.getStates().forEach(recolorNode));
}

function reachable_F(event) {
	selectedNodes = [];
	reachableNodes = [];
	cyGraph.cy.elements(":selected").each(function(elem){
		selectedNodes.push(elem.id());
		})

	switch (selectedNodes.length){
		case 0 :
			cyGraph.cy.$('#'+cyGraph.fa.initialState).select();
			reachableNodes = cyGraph.fa.reachable();
			break;
		case 1 :
			reachableNodes = canonical(cyGraph.fa.reachableX(selectedNodes[0],
																											cyGraph.fa.transitions));
			break;
		default :
			alert("More than one node selected");
			break;
	}

  reachableNodes.forEach(
		(node) => {
			cyGraph.cy.style()
				.selector(cyGraph.cy.filter(function(element, i){
					return !selectedNodes.includes(element.data('id'))
									&& element.isNode() && element.data('id') === node;
				}))
				.style({
					'background-color': 'purple'
				})
				.update()
			;
		}
	);
}

function productive_F(event) {
	productiveNodes = cyGraph.fa.productive();

  productiveNodes.forEach(
		(node) => {
			cyGraph.cy.style()
				.selector(cyGraph.cy.filter(function(element, i){
					return element.isNode() && element.data('id') === node;
				}))
				.style({
					'background-color': 'yellow'
				})
				.update()
			;
		}
	);
}

function useful_F(event) {
	f = cyGraph.fa;

	productiveNodes = f.productive();

	reachableNodes = f.reachable();

	usefulNodes = productiveNodes.filter(value => reachableNodes.includes(value));

	usefulNodes.forEach(
		(node) => {
			cyGraph.cy.style()
				.selector(cyGraph.cy.filter(function(element, i){
					return element.isNode() && element.data('id') === node;
				}))
				.style({
					'background-color': 'green'
				})
				.update()
			;
		}
	);
}

function arrayToString(arr){
  var str = "";
  for(i in arr) str += arr[i] + ", ";
  str = str.substr(0, str.length-2);
  return str;
}

function wordToString(arr){
  var str = "";
  for(i in arr) str += arr[i];
  return str;
}

function generate_F(event) {
	const n = document.getElementById('length').value;
	f = cyGraph.fa;
	result = f.generate(n).map(wordToString);
	document.getElementById('generateResult').innerHTML = arrayToString(result);
}

function accept_F(event) {
	const w = document.getElementById('word').value;

	if (cyGraph.fa.accept(w)) {
		document.getElementById('acceptResult').innerHTML = w + " is accepted";
  }
	else {
		document.getElementById('acceptResult').innerHTML = w + " is not accepted";
	}
}

function step_F(event) {
	var w = document.getElementById('word').value;
	f = cyGraph.fa;

	//get selected node
	var selectedNodes = [];
	cyGraph.cy.elements(":selected").each(function(elem){
		selectedNodes.push(elem.id());
		})

	switch (selectedNodes.length){
		case 0 :
			cyGraph.cy.$('#'+f.initialState).select();
			break;
		case 1 :
			break;
		default :
			alert("More than one node selected");
			return;
	}

	var selectedNode = selectedNodes[0];
	x = w.charAt(0);

	for(i in f.transitions){
		if(f.transitions[i][1] == x && f.transitions[i][0] == selectedNode){
			cyGraph.cy.$('#'+selectedNode).unselect();
			var next = f.transitions[i][2];
			cyGraph.cy.$('#'+next).select();

			w = w.substring(1);
			document.getElementById('word').value = w;
			if(w == "" && f.acceptStates.includes(next)){
				cyGraph.cy.style()
					.selector(cyGraph.cy.filter(function(element, i){
						return element.isNode() && element.data('id') === next;
					}))
					.style({
						'background-color': 'green'
					})
					.update()
				;
				return;
			}
			else if(w == ""){
				cyGraph.cy.style()
					.selector(cyGraph.cy.filter(function(element, i){
						return element.isNode() && element.data('id') === selectedNode;
					}))
					.style({
						'background-color': 'red'
					})
					.update()
				;
				return;
			}
			f.getStates().forEach(recolorNode);
			return;
		}
	}
	// if no transition available
	cyGraph.cy.style()
		.selector(cyGraph.cy.filter(function(element, i){
			return element.isNode() && element.data('id') === selectedNode;
		}))
		.style({
			'background-color': 'red'
		})
		.update()
	;
	return;
}

function animation_F(event) {setTimeout(function(){
	var w = document.getElementById('word').value;
	f = cyGraph.fa;

	//get selected node
	var selectedNodes = [];
	cyGraph.cy.elements(":selected").each(function(elem){
		selectedNodes.push(elem.id());
		})

	switch (selectedNodes.length){
		case 0 :
			cyGraph.cy.$('#'+f.initialState).select();
			break;
		case 1 :
			break;
		default :
			alert("More than one node selected");
			return;
	}

	var selectedNode = selectedNodes[0];
	x = w.charAt(0);

	transitionsAvailable = false;
	for(i in f.transitions){
			if(f.transitions[i][0] == selectedNode && f.transitions[i][1] == x){
			transitionsAvailable = true;
			break;
		}
	}
	if(w =! "" && transitionsAvailable){
		step_F(event);
		animation_F(event);
	}
},1000);}

function fileSelectAction(event) {
	const file = event.target.files[0];
	if( file == undefined ) // if canceled
		return;
	const reader = new FileReader();
	//callback function wird ausgeführt wenn reasAsFile abgeschlossen
	reader.onload = function(event) {
		cyGraph = CyGraph.load(event.target.result);
		statistics(cyGraph);
	};
	var input = event.srcElement;
  var fileName = input.files[0].name;
	document.getElementById('fileName').innerHTML = fileName;
	//reset input/output
	document.getElementById('generateResult').innerHTML = "";
	document.getElementById('length').value = "";
	document.getElementById('acceptResult').innerHTML = "";
	document.getElementById('word').value = "";
	reader.readAsText(file);
}

function statistics(graph){
	document.getElementById('states').innerHTML = graph.fa.getStates().length;
	document.getElementById('noAcceptStates').innerHTML =
		graph.fa.acceptStates.length;
	if(graph.fa.acceptStates.length != 1)
		document.getElementById('pluralS').innerHTML = "s";
	else document.getElementById('pluralS').innerHTML = "";
	document.getElementById('acceptStates').innerHTML =
		arrayToString(graph.fa.acceptStates);
  document.getElementById('alphabet').innerHTML =
		arrayToString(graph.fa.getAlphabet());
	document.getElementById('alphabetSize').innerHTML =
		graph.fa.getAlphabet().length;
	if(graph.fa.deterministic())
		document.getElementById('det').innerHTML = 'Deterministic';
	else document.getElementById('det').innerHTML = 'Non-deterministic';
}
