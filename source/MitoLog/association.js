/**
 * Global debug flag, useful for development.
 */
var _debug = false;

/**
 * Add array method to provide only the unique elements within the data.
 */
Array.prototype.unique = function ()
  {
    var o = {};
    var i;
    var l = this.length;
    var r = [];

    for (i = 0; i < l; i += 1)
      {
        o[this[i]] = this[i];
      }

    for (i in o)
      {
        r.push (o[i]);
      }

    return r;
  };

/**
 * Logger object for timestamping and displaying text into an element.
 */
Logger.prototype.constructor = Logger;

/**
 * Logger class definition.
 */
function Logger (name)
  {
    try
      {
        this.element = document.getElementById (name);
      }
    catch (e)
      {
        this.element = null;
      }

    /**
     * Clear the element being used to display the log entries.
     */
    Logger.prototype.clear = function ()
      {
        try
          {
            this.element.value = "";
          }
        catch (e)
          {
          }

        return;
      }

    /**
     * Add text to the element being used to display the log entries.
     */
    Logger.prototype.log = function (text)
      {
        stamp = new Date ().getTime ();
        try
          {
            if (this.element.value != "")
              {
                this.element.value = this.element.value + "\n";
              }

            this.element.value = this.element.value + stamp + ": " + text;
          }
        catch (e)
          {
          }

        return;
      }
  };

/**
 *
 */
Vector.prototype.constructor = Vector;

/**
 *
 */
function Vector (x, y)
  {
    this.x = x;
    this.y = y;

    /**
     *
     */
    Vector.prototype.add = function (other)
      {
        return (new Vector (this.x + other.x, this.y + other.y));
      }

    /**
     *
     */
    Vector.prototype.subtract = function (other)
      {
        return (new Vector (this.x - other.x, this.y - other.y));
      }

    /**
     *
     */
    Vector.prototype.multiply = function (x)
      {
        return (new Vector (this.x * x, this.y * x));
      }

    /**
     *
     */
    Vector.prototype.divide = function (x)
      {
        return (new Vector (this.x / x, this.y / x));
      }

    /**
     *
     */
    Vector.prototype.magnitude = function ()
      {
        return (Math.sqrt (this.x * this.x + this.y * this.y));
      }

    /**
     *
     */
    Vector.prototype.normalize = function ()
      {
        return (this.divide (this.magnitude ()));
      }
  }

/**
 *
 */
Node.prototype.constructor = Node;

/**
 *
 */
function Node (label, x, y, r, fill)
  {
    this.minsize = 20;
    this.maxsize = 30;
    this.font = 'bold 11pt sans-serif';

    this.selcolor = '#CC0000';
    this.selwidth = 3;

    this.neighbors = new Array ();

    /**
     *
     */
    Node.prototype.clip = function (context)
      {
        element = context.canvas;
        width = element.width;
        height = element.height;

        this.x = Math.max (this.radius, this.x);
        this.y = Math.max (this.radius, this.y);

        this.x = Math.min (width - this.radius, this.x);
        this.y = Math.min (height - this.radius, this.y);

        return;
      }

    /**
     *
     */
    Node.prototype.neighbor = function (other)
      {
        this.neighbors.push (other);

        return;
      }

    /**
     *
     */
    Node.prototype.label = function (label)
      {
        this.label = label;

        return (this.label);
      }

    /**
     *
     */
    Node.prototype.size = function (size)
      {
        this.radius = Math.max (this.minsize, size);
        this.radius = Math.min (this.maxsize, this.radius);

        return (this.radius);
      }

    /**
     *
     */
    Node.prototype.move = function (dx, dy)
      {
        this.x = this.x + dx;
        this.y = this.y + dy;

        return;
      }

    /**
     *
     */
    Node.prototype.randomize = function ()
      {
        scale = 2;

        this.move ((Math.random () - 0.5) * scale,
            (Math.random () - 0.5) * scale);

        return;
      }

    /**
     *
     */
    Node.prototype.select = function (context)
      {
        context.save ();
        context.strokeStyle = this.selcolor;
        context.lineWidth = this.selwidth;
        context.beginPath ();
        context.arc (this.x, this.y, this.radius + this.selwidth,
            0, Math.PI*2, true);
        context.closePath ();
        context.stroke ();
        context.restore ();

        return;
      }

    /**
     *
     */
    Node.prototype.draw = function (context)
      {
        this.clip (context);

        //
        // adjust the representation based on immediate neighbors
        //
        this.size (10 * this.neighbors.length);

        //
        // draw the basic shape
        //
        context.save ();
        context.fillStyle = this.fill;
        context.beginPath ();
        context.arc (this.x, this.y, Math.min (this.maxsize, this.radius),
            0, Math.PI*2, true);
        context.closePath ();
        context.fill ();
        context.restore ();

        //
        // add a selection highlight
        //
        if (this.selected)
          {
            this.select (context);
          }

        //
        // add labeling
        //
        context.save ();
        context.font = this.font;
        context.textAlign ='center';
        context.textBaseline ='middle';
        context.fillText (this.label, this.x, this.y);
        context.restore ();

        return;
      }

    /**
     *
     */
    Node.prototype.distance = function (other)
      {
        dx = this.x - other.x;
        dy = this.y - other.y;

        return (Math.sqrt (dx * dx + dy * dy));
      }

    /**
     *
     */
    Node.prototype.attract = function (other, context)
      {
        //
        // hold this node stable if selected
        //
        if (this.selected)
          {
            return;
          }

        d = this.distance (other);

        //
        // limit the influence of the attraction
        //
        if (d < ((this.radius + other.radius) + this.minsize))
          {
            return;
          }

        dx = this.x - other.x;
        dy = this.y - other.y;

        nx = -dx / d;
        ny = -dy / d;

        f = this.mass * d * d * d * d * d;
        f = Math.min (f, 10);

        tx = f * nx;
        ty = f * ny;

        if (other.selected)
          {
            this.force = this.force.add (new Vector (tx, ty));
          }
        else
          {
            this.force = this.force.add (new Vector (tx/2, ty/2));
            other.force = other.force.add (new Vector (-tx/2, -ty/2));
          }

        return;
      }

    /**
     *
     */
    Node.prototype.repel = function (other, context)
      {
        //
        // hold this node stable if selected
        //
        if (this.selected)
          {
            return;
          }

        d = this.distance (other);

        //
        // limit the influence of the repulsion
        //
        if (d >= ((this.radius + other.radius) * 2))
          {
            return;
          }

        //
        // randomize identical locations
        //
        if (d < 0.001)
          {
            this.randomize ();

            return;
          }

        dx = this.x - other.x;
        dy = this.y - other.y;

        nx = dx / d;
        ny = dy / d;

        f = 100 * (this.radius * other.radius) / (d * d);
        f = Math.min (f, 10);

        tx = f * nx;
        ty = f * ny;

        if (other.selected)
          {
            this.force = this.force.add (new Vector (tx, ty));
          }
        else
          {
            this.force = this.force.add (new Vector (tx/2, ty/2));
            other.force = other.force.add (new Vector (-tx/2, -ty/2));
          }

        return;
      }

    /**
     *
     */
    Node.prototype.update = function (context)
      {
        this.moving = false;

        if (this.force.magnitude () > 1)
          {
            this.mass = Math.max (1, this.neighbors.length);
            this.move (this.force.x / this.mass, this.force.y / this.mass);

            this.moving = true;
          }

        this.clip (context);

        return;
      }

    this.label = label;
    this.x = x;
    this.y = y;
    this.size (r);
    this.fill = fill;
    this.selected = false;
    this.mass = r;
    this.moving = true;

    this.position = new Vector (x, y);
    this.velocity = new Vector (0, 0);
    this.force = new Vector (0, 0);
  }

/**
 *
 */
Link.prototype.constructor = Link;

/**
 *
 */
function Link (a, b, label)
  {
    this.a = a;
    this.b = b;
    this.label = label;
    this.valid = false;

    if (a == b)
      {
        return;
      }

    if (a.neighbors.indexOf (b) != -1)
      {
        return;
      }

    if (b.neighbors.indexOf (a) != -1)
      {
        return;
      }

    a.neighbor (b);
    b.neighbor (a);

    this.valid = true;

    Link.prototype.draw = function (context)
      {
        if (this.valid)
          {
            context.save ();
            context.lineWidth = 5;
            context.strokeStyle = 'gray';
            context.beginPath ();
            context.moveTo (this.a.x, this.a.y);
            context.lineTo (this.b.x, this.b.y);
            context.stroke ();
            context.restore ();
          }

        return;
      }

    Link.prototype.annotate = function (context)
      {
        if (this.valid)
          {
            var cx = (this.a.x + this.b.x) / 2;
            var cy = (this.a.y + this.b.y) / 2;

            context.save ();
            context.font = 'bold 8pt sans-serif';
            context.textAlign ='center';
            context.textBaseline ='middle';

            //
            // draw a background to highlight the text
            //
            var border = 4;
            var metrics = context.measureText (this.label);
            var width = metrics.width;
            var height = 10;
            context.fillStyle = 'rgba(255,255,255,0.5)';
            context.fillRect (cx - (width + border) / 2, cy - (height + border)/ 2,
                width + border, height + border);

            //
            // draw the text
            //
            context.fillStyle = 'black';
            context.fillText (this.label, cx, cy);
            context.restore ();
          }

        return;
      }
  }

/**
 * Global list of all shapes and links to draw.
 */
var shapes = [];
var links = [];

/**
 * Clear the canvas.
 */
function clear (context)
  {
    element = context.canvas;

    width = element.width;
    height = element.height;

    context.save ();
    context.setTransform (1, 0, 0, 1, 0, 0);
    context.clearRect (0, 0, element.width, element.height);
    context.restore ();
  }

/**
 * Adjust the location of all of the shapes.
 */
function adjust (context)
  {
    moving = false;

    //
    // update the position of each shape
    //
    shapes.forEach (function (shape)
        {
          //
          // reset the net force acting on the shape
          //
          shape.force = new Vector (0, 0);

          //
          // attract immediate neighbors
          //
          shape.neighbors.forEach (function (neighbor)
            {
              shape.attract (neighbor, context);
            });

          //
          // repel all objects
          //
          shapes.forEach (function (other)
            {
              if (shape !== other)
                {
                  shape.repel (other, context);
                }
            });

          //
          // update the position of each shape
          //
          shape.update (context);

          moving = moving | shape.moving;

          canvasValid = true;
          if (moving)
            {
              invalidate ();
            }
        });

    if (_debug)
      {
        context.save ();
        context.font = '18pt sans-serif';
        context.textAlign ='left';
        context.textBaseline ='top';
        context.fillText ("moving: " + moving, 15, 10);
        context.restore ();
      }

    //
    // all done
    //
    return;
  }

/**
 * Redraw the shapes if there has been a change in their position or an
 * a mouse interaction with them.
 */
function refresh (context)
  {
    //
    // make sure the canvas resizes automatically
    //
    var canvas = context.canvas;
    if (canvas.width != canvas.parentElement.offsetWidth)
      {
        canvas.width = canvas.parentElement.offsetWidth;
      }

    //
    // only re-draw if needed
    //
    if (canvasValid)
      {
        return;
      }

    //
    // clear the drawing area
    //
    clear (context);

    //
    // draw the interconnections
    //
    links.forEach (function (link)
        {
          link.draw (context);
        });

    //
    // draw the nodes
    //
    shapes.forEach (function (shape)
        {
          shape.draw (context);
        });

    //
    // annotate the interconnections
    //
    links.forEach (function (link)
        {
          link.annotate (context);
        });

    //
    // adjust the shape locations
    //
    adjust (context);

    //
    // all done
    //
    return;
  }

// ==========================================================================

var canvas;
var ctx;
// how often, in milliseconds, we check to see if a redraw is needed
var INTERVAL = 20;
var timer = null;
var poll = null;

var isDrag = false;
var mx, my; // mouse coordinates

// when set to true, the canvas will redraw everything
// invalidate() just sets this to false right now
// we want to call invalidate() whenever we make a change
var canvasValid = false;

// The node (if any) being selected.
// If in the future we want to select multiple objects, this will get turned into an array
var selected = null;

// we use a fake canvas to draw individual shapes for selection testing
var ghostcanvas;
var gctx; // fake canvas context

// since we can drag from anywhere in a node
// instead of just its x/y corner, we need to save
// the offset of the mouse when we start dragging.
var offsetx, offsety;

// Padding and border style widths for mouse offsets
var stylePaddingLeft, stylePaddingTop, styleBorderLeft, styleBorderTop;

/**
 * Mouse movement event callback.
 */
function myMove (event)
  {
    if (isDrag)
      {
        getMouse(event);

        selected.x = mx - offsetx;
        selected.y = my - offsety;

        invalidate ();
      }
  }

/**
 * Mouse down event callback.
 */
function myDown (event)
  {
    getMouse (event);
    clear (gctx);

    shapes.forEach (function (shape)
        {
          shape.selected = false;
        });

    temp = shapes.slice (0);
    temp.reverse ();
    temp.forEach (function (shape)
        {
        });


    var l = shapes.length;

    for (var i = l-1; i >= 0; i--)
      {
        shapes[i].draw (gctx);

        // get image data at the mouse x,y pixel
        var imageData = gctx.getImageData (mx, my, 1, 1);
        var index = (mx + my * imageData.width) * 4;

        // if the mouse pixel exists, select and break
        if (imageData.data[3] > 0)
          {
            selected = shapes[i];
            selected.selected = true;

            offsetx = mx - selected.x;
            offsety = my - selected.y;
            selected.x = mx - offsetx;
            selected.y = my - offsety;

            isDrag = true;
            canvas.onmousemove = myMove;

            invalidate ();

            return;
          }
      }

    // havent returned means we have selected nothing
    selected = null;
  }

function myUp (event)
  {
    isDrag = false;

    canvas.onmousemove = null;

    if (selected)
      {
        selected.selected = false;
        invalidate ();
      }

  }

/**
 * Mouse double-click event callback.
 */
function myDblClick (event)
  {
    return;
  }

/**
 * Invalidate the display to force a redraw on the next cycle.
 */
function invalidate ()
  {
    canvasValid = false;
  }

// Sets mx,my to the mouse position relative to the canvas
// unfortunately this can be tricky, we have to worry about padding and borders
function getMouse (event)
  {
    var element = canvas, offsetX = 0, offsetY = 0;

    if (element.offsetParent)
      {
        do
          {
            offsetX += element.offsetLeft;
            offsetY += element.offsetTop;
          }
        while ((element = element.offsetParent));
      }

    // Add padding and border style widths to offset
    offsetX += stylePaddingLeft;
    offsetY += stylePaddingTop;

    offsetX += styleBorderLeft;
    offsetY += styleBorderTop;

    mx = event.pageX - offsetX;
    my = event.pageY - offsetY
  }

/**
 * Render XML data as an association diagram.
 */
function association (xml, element)
  {
    canvas = document.getElementById(element);
    ctx = canvas.getContext('2d');

    ghostcanvas = document.createElement('canvas');
    ghostcanvas.height = canvas.height;
    ghostcanvas.width = canvas.width;
    gctx = ghostcanvas.getContext('2d');

    //fixes a problem where double clicking causes text to get selected on the canvas
    canvas.onselectstart = function ()
      {
        return (false);
      }

    // fixes mouse co-ordinate problems when there's a border or padding
    // see getMouse for more detail
    if (document.defaultView && document.defaultView.getComputedStyle)
      {
        stylePaddingLeft =
          parseInt (document.defaultView.getComputedStyle (canvas, null)['paddingLeft'], 10)      || 0;
        stylePaddingTop  =
          parseInt(document.defaultView.getComputedStyle(canvas, null)['paddingTop'], 10)       || 0;
        styleBorderLeft  =
          parseInt(document.defaultView.getComputedStyle(canvas, null)['borderLeftWidth'], 10)  || 0;
        styleBorderTop   =
          parseInt(document.defaultView.getComputedStyle(canvas, null)['borderTopWidth'], 10)   || 0;
      }

    // set our events. Up and down are for dragging,
    // double click is for making new shapes
    canvas.onmousedown = myDown;
    canvas.onmouseup = myUp;
    canvas.ondblclick = myDblClick;

    // add custom initialization here:

    xc = canvas.width / 2;
    yc = canvas.height / 2;

    var data = xml.getElementsByTagName ("association")[0];

    //
    // clear any association between the nodes
    //
    links = [];
    shapes.forEach (function (shape)
        {
          shape.neighbors = [];
        });

    //
    // find the unique node names in the data
    //
    var temp = new Array ();
    for (loop = 0; loop < data.childNodes.length; loop++)
      {
        temp.push (data.childNodes[loop].getAttribute("sender"));
        temp.push (data.childNodes[loop].getAttribute("receiver"));
      }

    names = temp.unique ();
    shapes.forEach (function (shape)
        {
          var index = names.indexOf (shape.label);
          if (index != -1)
            {
              names.splice (index,1);
            }
        });

    //
    // create the unique nodes
    //
    for (loop = 0; loop < names.length; loop++)
      {
        shapes.push (new Node (names[loop], xc, yc, 1, 'green'));
      }

    //
    // link the nodes based on the association data
    //
    for (loop = 0; loop < data.childNodes.length; loop++)
      {
        var node = data.childNodes[loop];

        var a = -1;
        for (index = 0; index < shapes.length; index++)
          {
            if (node.getAttribute("sender") == shapes[index].label)
              {
                a = index;
                break;
              }
          }

        var b = -1;
        for (index = 0; index < shapes.length; index++)
          {
            if (node.getAttribute("receiver") == shapes[index].label)
              {
                b = index;
                break;
              }
          }

        var label = node.getAttribute("count");
        
        if ((a > -1) && (b > -1))
          {
            links.push (new Link (shapes[a], shapes[b], label));
          }
      }

    //
    // highlight any nodes with an alarm status
    //
    data = xml.getElementsByTagName ("alarms")[0];

    for (loop = 0; loop < data.childNodes.length; loop++)
      {
        try
          {
            var node = data.childNodes[loop];
            var actor = node.getAttribute ("actor");
            var condition = node.getAttribute ("condition");

            shapes.forEach (function (shape)
                {
                  {
                    if (shape.label == actor)
                      {
                        if (condition == 0)
                          {
                            shape.fill = 'green';
                          }
                        else
                          {
                            shape.fill = 'orange';
                          }
                      }
                  }
                });
          }
        catch (e)
          {
          }
      }

    invalidate ();

    //
    // request to be triggered to update the association data from the target
    //
    if (poll)
      {
        clearTimeout (poll);
      }

    var element = document.getElementById ("delay");
    var delay = parseInt (element.value);
    delay = Math.max (1.0, delay);

    poll = setTimeout (function ()
        {
          fetch ('/association.xml', 'diagram', association);
        }, delay * 1000);

    //
    // periodically update the visual to enable mouse interaction and animation
    //
    if (timer)
      {
        clearInterval (timer);
      }

    timer = setInterval (function ()
        {
          refresh (ctx);
        }, INTERVAL);

    return;
  }

/**
 * Fetch XML data from a source and render it graphically.
 */
function fetch (url, element, renderer)
  {
    var debug = new Logger ("debug");

    var request = false;
    if (window.XMLHttpRequest)
      {
        request = new XMLHttpRequest ();
      }
    else
      {
        request = new ActiveXObject ("Microsoft.XMLHTTP");
      }

    request.onreadystatechange = function ()
      {
        if ((request.readyState == 4) && (request.status == 200))
          {
            debug.log ("response");
            renderer (request.responseXML, element);
          }
      }

    window.status = "loading data from service...";
    debug.log ("request");

    request.open ("GET", url + '?cache=' + new Date ().getTime (), true);
    request.send ();

    return;
  }
