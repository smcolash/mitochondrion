//
// Declare a namespace for the code in this module.
//
var mito = mito || {};

/**
 *
 */
History.prototype.constructor = History;

/**
 *
 */
function History ()
  {
    this._active = 0;
    this._timestamp = 0;
    this._text = "";

    /**
     *
     */
    History.prototype.timestamp = function ()
      {
        return (this._timestamp);
      };

    /**
     *
     */
    History.prototype.active = function ()
      {
        return (this._active);
      };

    /**
     *
     */
    History.prototype.text = function ()
      {
        return (this._text);
      };

    /**
     *
     */
    History.prototype.draw_timestamp = function (context, y)
      {
        context.save ();

        context.font = 'normal 9pt sans-serif';
        context.fillStyle = 'black';
        context.textBaseline = 'middle';
        context.fillText (this.timestamp ().toFixed (6),
            History.labelOffset, y);

        context.restore ();

        return;
      };

    /**
     *
     */
    History.prototype.draw_description = function (context, y)
      {
        context.save ();

        context.font = 'normal 9pt sans-serif';
        context.fillStyle = 'black';
        context.textBaseline = 'middle';
        context.fillText (this.text (), History.timeStampWidth, y);

        context.restore ();

        return;
      };

    /**
     *
     */
    History.prototype.draw_lanes = function (context, y)
      {
        context.save ();

        for (var loop = 0; loop < History.lanes.length; loop++)
          {
            var x = History.labelWidth + (loop + 0.5) * History.laneWidth;

            var color = 'lightgrey';
            var width = 4;

            if (History.lanes[loop] == this.active ())
              {
                color = 'red';
                width = 8;
              }

            context.strokeStyle = color;
            context.fillStyle = color;

            if (History.locked[History.lanes[loop]] == true)
              {
                width = 2;

                context.fillRect (x - 6 / 2, y - 6 / 2, 6, 6);
              }

            if ((History.alarm[History.lanes[loop]] != null) &&
                (History.alarm[History.lanes[loop]] != 0))
              {
                context.save ();
                context.fillStyle = 'orange';
                context.strokeStyle = 'orange';
                context.fillRect (x - 10, y - History.height / 2 + 4,
                    4, History.height - 8);
                context.restore ();
              }

            context.lineWidth = width;

            context.beginPath ();
            context.moveTo (x, y - (History.height / 2));
            context.lineTo (x, y + (History.height / 2));
            context.stroke ();
          }

        context.restore ();

        return;
      }

    /**
     *
     */
    History.prototype.decorate = function (context, y)
      {
        return;
      }

    /**
     *
     */
    History.prototype.draw = function (context, y)
      {
        this.draw_timestamp (context, y);
        this.draw_description (context, y);
        this.draw_lanes (context, y);
        this.decorate (context, y);

        return;
      }
  };

/**
 *
 */
History.font = 'normal 9pt sans-serif';

/**
 *
 */
History.height = 18;

/**
 *
 */
History.width = 2;

/**
 *
 */
History.timeStampWidth = 130;

/**
 *
 */
History.labelWidth = History.timeStampWidth + 300;

/**
 *
 */
History.labelOffset = 5;

/**
 *
 */
History.laneWidth = 80;

/**
 *
 */
History.actors = new Array ();

/**
 *
 */
History.actor = function (name)
  {
    History.actors[name] = 1;

    return;
  };

/**
 *
 */
History.names = function ()
  {
    var lanes = new Array ();
    for (var key in History.actors)
      {
        lanes.push (key);
      }

    lanes.sort ();

    return (lanes);
  };

/**
 *
 */
History.lanes = new Array ();

/**
 *
 */
History.lookup = new Array ();

/**
 *
 */
History.locked = new Array ();

/**
 *
 */
History.alarm = new Array ();

/**
 *
 */
History.prepare = function (initial)
  {
    if (initial)
      {
        History.lanes = new Array ();
        for (var key in History.actors)
          {
            History.lanes.push (key);
          }

        History.lanes.sort ();
      }

    for (var index = 0; index < History.lanes.length; index++)
      {
        History.lookup[History.lanes[index]] = index;
      }

    return;
  };

/**
 *
 */
History.events = new Array ();

/**
 *
 */
History.register = function (name, value)
  {
    History.events[value] = name;

    return;
  }

/**
 *
 */
Header.prototype = new History ();

/**
 *
 */
Header.prototype.constructor = Header;

/**
 *
 */
Header.superclass = History.prototype;

/**
 *
 */
function Header ()
  {
    History.apply (this, arguments);

    /**
     *
     */
    Header.prototype.draw = function (context, y)
      {
        context.save ();

        var base = Math.floor (History.height / 2);

        context.font = 'bold 11pt sans-serif';
        context.fillStyle = 'black';
        context.textBaseline = 'middle';
        context.textAlign = 'left';

        context.fillText ('Timestamp', History.labelOffset, base);
        context.fillText ('Description', History.timeStampWidth, base);

        context.textAlign = 'center';
        for (var loop = 0; loop < History.lanes.length; loop++)
          {
            context.fillText (History.lanes[loop],
                History.labelWidth + (loop + 0.5) * History.laneWidth, base);
          }

        context.restore ();

        return;
      };
  };

/**
 *
 */
Separator.prototype = new History ();

/**
 *
 */
Separator.prototype.constructor = Separator;

/**
 *
 */
Separator.superclass = History.prototype;

/**
 *
 */
function Separator ()
  {
    History.apply (this, arguments);

    /**
     *
     */
    this.separatorColor = 'black';

    /**
     *
     */
    this.separatorOffset = History.labelOffset;

    /**
     *
     */
    Separator.prototype.draw = function (context, y)
      {
        context.save ();

        x0 = this.separatorOffset;
        x1 = History.labelWidth +
          (History.lanes.length * History.laneWidth) - this.separatorOffset;

        context.save ();

        context.strokeStyle = this.separatorColor;
        context.fillStyle = this.separatorColor;

        context.lineWidth = this.lineWidth;

        context.beginPath ();
        context.moveTo (x0, y);
        context.lineTo (x1, y);
        context.closePath ();
        context.stroke ();

        context.restore ();

        return;
      };
  };

/**
 *
 */
Event.prototype = new History ();

/**
 *
 */
Event.prototype.constructor = Event;

/**
 *
 */
Event.superclass = History.prototype;

/**
 *
 */
function Event (timestamp, active, code, from, to)
  {
    History.apply (this, arguments);

    this.arrowHeight = 5;
    this.arrowWidth = 11;
    this.arrowColor = 'black';
    this.lineWidth = 2;

    this._timestamp = parseFloat (timestamp);
    this._active = active;
    this.code = code;
    this.from = from;
    this.to = to;

    History.actor (active);
    History.actor (from);
    History.actor (to);

    /*
     * 
     */
    Event.prototype.decorate = function (context, y)
      {
        x0 = History.labelWidth +
          (History.lookup[this.from] + 0.5) * History.laneWidth;
        x1 = History.labelWidth +
          (History.lookup[this.to] + 0.5) * History.laneWidth;

        var sign = (x1 - x0) / Math.abs (x1 - x0);

        context.save ();

        context.strokeStyle = this.arrowColor;
        context.fillStyle = this.arrowColor;

        //
        // draw the shaft of the arrow only for send
        //
        if (this._active == this.from)
          {
            context.lineWidth = this.lineWidth;

            context.beginPath ();
            context.moveTo (x0, y);
            context.lineTo (x1 - this.arrowWidth * sign, y);
            context.closePath ();
            context.stroke ();
          }

        //
        // draw the head of the arrow for send and receive
        //
        context.lineWidth = 1;
        context.lineJoin = 'miter';
        context.beginPath ();
        context.moveTo (x1, y);
        context.lineTo (x1-(sign * this.arrowWidth), y+this.arrowHeight);
        context.lineTo (x1-(sign * this.arrowWidth), y-this.arrowHeight);
        context.fill ();

        context.restore ();

        return;
      }

    /*
     * 
     */
    Event.prototype.text = function ()
      {
        var mode = "sent";
        if (this._active == this.to)
          {
            mode = "received";
          }

        return ("'" + History.events[this.code] + "' " + mode + " from '" + this.from + "'");
      }
  };

/**
 *
 */
Transition.prototype = new History ();

/**
 *
 */
Transition.prototype.constructor = Transition;

/**
 *
 */
Transition.superclass = History.prototype;

/**
 *
 */
function Transition (timestamp, active, from, to)
  {
    History.apply (this, arguments);

    this._timestamp = parseFloat (timestamp);
    this._active = active;
    this.from = from;
    this.to = to;

    this.radius = 3;

    History.actor (active);

    /**
     *
     */
    Transition.prototype.xc = function (active)
      {
        var x = History.labelWidth +
          (History.lookup[active] + 0.5) * History.laneWidth;

        return (x);
      }

    /**
     *
     */
    Transition.prototype.decorate = function (context, y)
      {
        context.lineWidth = 1;
        context.strokeStyle = 'green';
        context.fillStyle = 'green';

        x = this.xc (this.active ());

        dx = 24;
        dy = (History.height / 2) - 2;

        context.beginPath ();
        context.moveTo (x - dx, y - dy);
        context.lineTo (x + dx, y - dy);
        context.lineTo (x + dx + dy, y);
        context.lineTo (x + dx, y + dy);
        context.lineTo (x - dx, y + dy);
        context.lineTo (x - dx - dy, y);
        context.closePath ();
        context.stroke;
        context.fill ();

        return;
      }

    /**
     *
     */
    Transition.prototype.text = function ()
      {
        return ("state transition from '" + this.from + "' to '" + this.to + "'");
      }
  };

/**
 *
 */
Timer.prototype = new History ();

/**
 *
 */
Timer.prototype.constructor = Timer;

/**
 *
 */
Timer.superclass = History.prototype;

/**
 *
 */
function Timer ()
  {
    History.apply (this, arguments);

    this.radius = 3;

    /**
     *
     */
    Timer.prototype.xc = function (active)
      {
        var x = History.labelWidth +
          (History.lookup[active] + 0.5) * History.laneWidth;
        var xc = x - (8 * this.radius);

        return (xc);
      }

    /**
     *
     */
    Timer.prototype.draw_timer = function (context, y, active)
      {
        context.save ();

        context.lineWidth = 1;
        context.strokeStyle = 'black';
        context.fillStyle = 'white';

        x = this.xc (active);

        context.beginPath ();
        context.arc (x, y, 2 * this.radius, 0, 2 * Math.PI, true);
        context.stroke ();
        context.fill ();

        context.lineJoin = 'miter';
        context.moveTo (x, y - this.radius - 1);
        context.lineTo (x, y);
        context.lineTo (x + this.radius, y);
        context.stroke ();

        context.restore ();

        return;
      }
  };

/**
 *
 */
TimerSet.prototype = new Timer ();

/**
 *
 */
TimerSet.prototype.constructor = TimerSet;

/**
 *
 */
TimerSet.superclass = Timer.prototype;

/**
 *
 */
function TimerSet (timestamp, active, name, value)
  {
    History.apply (this, arguments);

    this._timestamp = parseFloat (timestamp);
    this._active = active;
    this.name = name;
    this.value = value;

    /**
     *
     */
    TimerSet.prototype.decorate = function (context, y)
      {
        this.draw_timer (context, y, this.active ());

        context.save ();

        context.strokeStyle = 'black';

        var x = this.xc (this.active ());

        context.moveTo (x - 4 * this.radius - this.radius, y);
        context.lineTo (x - 4 * this.radius + this.radius, y);
        context.stroke ();

        context.moveTo (x - 4 * this.radius, y - this.radius);
        context.lineTo (x - 4 * this.radius, y + this.radius);
        context.stroke ();

        context.restore ();

        return;
      }

    /**
     *
     */
    TimerSet.prototype.text = function ()
      {
        return ("'" + this.name + "' set to '" + this.value + "'");
      }
  };

/**
 *
 */
TimerCancel.prototype = new Timer ();

/**
 *
 */
TimerCancel.prototype.constructor = TimerCancel;

/**
 *
 */
TimerCancel.superclass = Timer.prototype;

/**
 *
 */
function TimerCancel (timestamp, active, name)
  {
    History.apply (this, arguments);

    this._timestamp = parseFloat (timestamp);
    this._active = active;
    this.name = name;

    /**
     *
     */
    TimerCancel.prototype.decorate = function (context, y)
      {
        this.draw_timer (context, y, this.active ());

        context.save ();

        context.strokeStyle = 'black';

        var x = this.xc (this.active ());

        context.moveTo (x - 4 * this.radius - this.radius, y);
        context.lineTo (x - 4 * this.radius + this.radius, y);
        context.stroke ();

        context.restore ();

        return;
      }

    /**
     *
     */
    TimerCancel.prototype.text = function ()
      {
        return ("'" + this.name + "' cancelled");
      }
  };

/**
 *
 */
TimerExpire.prototype = new Timer ();

/**
 *
 */
TimerExpire.prototype.constructor = TimerExpire;

/**
 *
 */
TimerExpire.superclass = Timer.prototype;

/**
 *
 */
function TimerExpire (timestamp, active, name)
  {
    History.apply (this, arguments);

    this._timestamp = parseFloat (timestamp);
    this._active = active;
    this.name = name;

    /**
     *
     */
    TimerExpire.prototype.decorate = function (context, y)
      {
        this.draw_timer (context, y, this.active ());

        context.save ();

        context.strokeStyle = 'red';

        var x = this.xc (this.active ());
        var span = 0.1875;

        context.beginPath ();
        context.arc (x, y, 3 * this.radius,
            (0.0 + span) * Math.PI, (0.0 - span) * Math.PI, true);
        context.stroke ();

        context.beginPath ();
        context.arc (x, y, 3 * this.radius,
            (1.0 + span) * Math.PI, (1.0 - span) * Math.PI, true);
        context.stroke ();

        context.beginPath ();
        context.arc (x, y, 4 * this.radius,
            (0.0 + span) * Math.PI, (0.0 - span) * Math.PI, true);
        context.stroke ();

        context.beginPath ();
        context.arc (x, y, 4 * this.radius,
            (1.0 + span) * Math.PI, (1.0 - span) * Math.PI, true);
        context.stroke ();

        context.restore ();

        return;
      }

    /**
     *
     */
    TimerExpire.prototype.text = function ()
      {
        return ("'" + this.name + "' expired");
      }
  };

/**
 *
 */
Text.prototype = new History ();

/**
 *
 */
Text.prototype.constructor = Text;

/**
 *
 */
Text.superclass = History.prototype;

/**
 *
 */
function Text (timestamp, active, description)
  {
    History.apply (this, arguments);

    History.actor (active);

    this._timestamp = parseFloat (timestamp);
    this._active = active;
    this._text = description;
  };

/**
 *
 */
MutexTake.prototype = new History ();

/**
 *
 */
MutexTake.prototype.constructor = MutexTake;

/**
 *
 */
MutexTake.superclass = History.prototype;

/**
 *
 */
function MutexTake (timestamp, active, name)
  {
    History.apply (this, arguments);

    History.actor (active);

    this._timestamp = parseFloat (timestamp);
    this._active = active;
    this._name = name;

    /**
     *
     */
    MutexTake.prototype.draw = function (context, y)
      {
        History.locked[this.active ()] = true;
        MutexTake.superclass.draw.call (this, context, y);
        History.locked[this.active ()] = false;
      }

    /**
     *
     */
    MutexTake.prototype.text = function ()
      {
        return ("resource '" + this._name + "' was taken");
      };
  };

/**
 *
 */
MutexGive.prototype = new History ();

/**
 *
 */
MutexGive.prototype.constructor = MutexGive;

/**
 *
 */
MutexGive.superclass = History.prototype;

/**
 *
 */
function MutexGive (timestamp, active, name)
  {
    History.apply (this, arguments);

    History.actor (active);

    this._timestamp = parseFloat (timestamp);
    this._active = active;
    this._name = name;

    /**
     *
     */
    MutexGive.prototype.draw = function (context, y)
      {
        History.locked[this.active ()] = true;
        MutexGive.superclass.draw.call (this, context, y);
        History.locked[this.active ()] = false;
      }

    /**
     *
     */
    MutexGive.prototype.text = function ()
      {
        return ("resource '" + this._name + "' was released");
      };
  };

/**
 *
 */
AlarmClear.prototype = new History ();

/**
 *
 */
AlarmClear.prototype.constructor = AlarmClear;

/**
 *
 */
AlarmClear.superclass = History.prototype;

/**
 *
 */
function AlarmClear (timestamp, active, condition)
  {
    History.apply (this, arguments);

    History.actor (active);

    this._timestamp = parseFloat (timestamp);
    this._active = active;
    this._condition = condition;

    /**
     *
     */
    AlarmClear.prototype.draw = function (context, y)
      {
        History.alarm[this.active ()] = 0;
        AlarmClear.superclass.draw.call (this, context, y);
      }

    /**
     *
     */
    AlarmClear.prototype.text = function ()
      {
        return ("alarm condition was cleared");
      };
  };

/**
 *
 */
AlarmSet.prototype = new History ();

/**
 *
 */
AlarmSet.prototype.constructor = AlarmSet;

/**
 *
 */
AlarmSet.superclass = History.prototype;

/**
 *
 */
function AlarmSet (timestamp, active, condition)
  {
    History.apply (this, arguments);

    History.actor (active);

    this._timestamp = parseFloat (timestamp);
    this._active = active;
    this._condition = condition;

    /**
     *
     */
    AlarmSet.prototype.draw = function (context, y)
      {
        History.alarm[this.active ()] = this._condition;
        AlarmSet.superclass.draw.call (this, context, y);
      }

    /**
     *
     */
    AlarmSet.prototype.text = function ()
      {
        return ("alarm condition '" + this._condition + "' was set");
      };
  };

/**
 *
 */
Unsupported.prototype = new History ();

/**
 *
 */
Unsupported.prototype.constructor = Header;

/**
 *
 */
Unsupported.superclass = History.prototype;

/**
 *
 */
function Unsupported (timestamp, active, type)
  {
    History.apply (this, arguments);

    this._timestamp = parseFloat (timestamp);
    this._active = active;
    this._text = "unsupported: " + type;
  };

/**
 *
 */
function redraw (element, records)
  {
    element.style.cursor = "progress";

    //
    // create a visual representation of the history data
    //
    if (element.getContext)
      {
        var context = element.getContext('2d');

        element.width = History.labelWidth +
          History.lanes.length * History.laneWidth;
        element.height = records.length * History.height;

        //
        // process each record into its visual representation
        //
        var length = records.length;
        for (var index = 0; index < length; index++)
          {
            var y = Math.floor ((History.height / 2) +
                (index * History.height));
            records[index].draw (context, y);
          }
      }
    else
      {
        element.innerHTML='no html5 canvas support';
      }

    window.status = '';

    element.style.cursor = "default";

    return;
  }

/**
 *
 */
function msc (xml, element)
  {
    canvas = document.getElementById (element);

    var start = new Date ().getTime ();

    var catalog = xml.getElementsByTagName ("catalog")[0];
    var events = xml.getElementsByTagName ("events")[0];

    var records = new Array ();

    window.status = 'constructing sequence diagram...';

    //
    // create a catalog of event names and codes
    //
    for (var loop = 0; loop < catalog.childNodes.length; loop++)
      {
        try
          {
            var name = catalog.childNodes[loop].getAttribute ("name");
            var code = catalog.childNodes[loop].getAttribute ("code");

            History.register (name, code);
          }
        catch (error)
          {
          }
      }

    //
    // start the list of records with a header
    //
    records.push (new Header ());

    //
    // process the raw history data into a set of records
    //
    for (i = 0; i < events.childNodes.length; i++)
      {
        try
          {
            var type = events.childNodes[i].getAttribute ("type");
            var timestamp = events.childNodes[i].getAttribute ("timestamp");
            var active = events.childNodes[i].getAttribute ("context");

            if ((type == 'EVENT_SEND') || (type == 'EVENT_RECEIVE'))
              {
                code = events.childNodes[i].getAttribute ("code");
                from = events.childNodes[i].getAttribute ("from");
                to = events.childNodes[i].getAttribute ("to");
                records.push (new Event (timestamp, active, code, from, to));
              }
            else if (type == 'TRANSITION')
              {
                from = events.childNodes[i].getAttribute ("from");
                to = events.childNodes[i].getAttribute ("to");
                records.push (new Transition (timestamp, active, from, to));
              }
            else if (type == 'TIMER_SET')
              {
                name = events.childNodes[i].getAttribute ("name");
                value = events.childNodes[i].getAttribute ("value");
                records.push (new TimerSet (timestamp, active, name, value));
              }
            else if (type == 'TIMER_CANCEL')
              {
                code = events.childNodes[i].getAttribute ("name");
                records.push (new TimerCancel (timestamp, active, name));
              }
            else if (type == 'TIMER_EXPIRE')
              {
                name = events.childNodes[i].getAttribute ("name");
                records.push (new TimerExpire (timestamp, active, name));
              }
            else if (type == 'MUTEX_TAKE')
              {
                name = events.childNodes[i].getAttribute ("name");
                records.push (new MutexTake (timestamp, active, name));
              }
            else if (type == 'MUTEX_GIVE')
              {
                name = events.childNodes[i].getAttribute ("name");
                records.push (new MutexGive (timestamp, active, name));
              }
            else if (type == 'ALARM_CLEAR')
              {
                condition = events.childNodes[i].getAttribute ("condition");
                records.push (new AlarmClear (timestamp, active, condition));
              }
            else if (type == 'ALARM_SET')
              {
                condition = events.childNodes[i].getAttribute ("condition");
                records.push (new AlarmSet (timestamp, active, condition));
              }
            else if (type == 'TEXT')
              {
                text = events.childNodes[i].getAttribute ("text");
                records.push (new Text (timestamp, active, text));
              }
            else if (type == 'CODE')
              {
                text = events.childNodes[i].getAttribute ("text");
                records.push (new Code (timestamp, active, text));
              }
            else
              {
                text = events.childNodes[i].getAttribute ("payload");
                records.push (new Unsupported (timestamp, active, text));
              }
          }
        catch (error)
          {
          }
      }

    //
    // display the initial view of the data
    //
    History.prepare (true);
    redraw (canvas, records);

    //
    // set event handlers on the canvas
    //
    var down = false;
    var source = null;
    var dest = null;

    canvas.addEventListener ('mousedown',
        function (event)
          {
            if (event.shiftKey)
              {
                index = Math.floor (event.layerY / History.height);
                index = Math.max (0, index);

                if (index == 0)
                  {
                    return (false);
                  }

                records.splice (index, 1);

                History.prepare (false);
                redraw (canvas, records);

                return (false);
              }

            down = true;
            source = Math.floor ((event.layerX - History.labelWidth) / History.laneWidth);
            dest = null;

            canvas.style.cursor="move";

            return (false);
          }, false);

    canvas.addEventListener ('mouseup',
        function (event)
          {
            canvas.style.cursor="default";

            if (!down)
              {
                down = false;
                source = null;
                dest = null;

                return (false);
              }

            dest = Math.floor ((event.layerX - History.labelWidth) / History.laneWidth);

            if ((source == null) || (dest == null))
              {
                down = false;
                source = null;
                dest = null;

                return (false);
              }

            source = Math.max (0, source);
            source = Math.min (History.lanes.length - 1, source);

            dest = Math.max (0, dest);
            dest = Math.min (History.lanes.length - 1, dest);

            if (source == dest)
              {
                down = false;
                source = null;
                dest = null;

                return (false);
              }

            temp = History.lanes[source];
            History.lanes.splice (source, 1);
            History.lanes.splice (dest, 0, temp);

            History.prepare (false);
            redraw (canvas, records);

            down = false;
            source = null;
            dest = null;

            return (false);
          }, false);

if (0)
{
    canvas.addEventListener ('mousemove',
        function (event)
          {
            return (false);
          }, false);
}

    canvas.addEventListener ('mouseout',
        function (event)
          {
            down = false;
            source = null;
            dest = null;

            return (false);
          }, false);

    canvas.addEventListener ('dblclick',
        function (event)
          {
            index = Math.floor (event.layerY / History.height);
            index = Math.max (0, index);

            if (index == 0)
              {
                return (false);
              }

            if (event.shiftKey)
              {
                records.splice (index, 1);
              }
            else if (records[index] instanceof Separator)
              {
                records.splice (index, 1);
              }
            else
              {
                records.splice (index, 0, new Separator ());
              }

            History.prepare (false);
            redraw (canvas, records);

            return (false);
          }, false);

    return;
  }

/**
 * Fetch XML data from a source and render it graphically.
 */
function fetch (url, element, renderer)
  {
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
            renderer (request.responseXML, element);
          }
      }

    window.status = "loading data from service...";

    request.open ("GET", url + '?cache=' + new Date ().getTime (), true);
    request.send ();

    return;
  }
