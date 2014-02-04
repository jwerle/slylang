
function Type (o) {
}

function Node (o) {
  this.type = o.type;
  this.value = o.value;
}

function Token (o) {
  this.type = o.type;
  this.node = o.node
}

/**
 *
 * require 'string'
 * require 'file'
 *
 * define FOO "bar"
 *
 * typedef Person {
 *  name;
 *  age;
 *  email;
 * }
 *
 * let Person (name) {
 *  ::name = name;
 *  ::age = 0;
 *  ::email = "";
 * }
 *
 * typedef List {
 *  head;
 *  tail;
 * }
 *
 * let List () {
 * }
 *
 * let List::next () {
 *  ::head
 * }
 *
 * let fn (a, b) {
 *  return a + b;
 * }
 *
 * let main (argc, argv) {
 *  key = "value";
 *  joe = Person::new();
 *
 *  return 0;
 *  }
 */

function code (ch) {
  return ch.charCodeAt(0);
}

function isspace (ch) {
  return ((code(ch) == 0x09) || (code(ch) == 0x0a) ||
          (code(ch) == 0x0c) || (code(ch) == 0x0d) ||
          (code(ch) == 0x020));
}

function isascii (ch) {
  return (code(ch) >= 0x0041) && (code(ch) <= 0x7a);
}

var RESERVED_VAR_CHARS = [
  '!', '#', '%', '^', '&',
  '*', '(', ')', '`', '~',
  '"', ':', ';', ',', '.',
  '<', '>', '/', '?', '|',
  '{', '}', '[', ']', '\\',
  '0', '1', '2', '3', '4',
  '5', '6', '7', '8', '9',
  ' '
];

var CLEAN_WHITE_SPACE_STATE = 0;
var BEGIN_STATE = 1;
var DEFINE_BEGIN_STATE = 2;
var DEFINE_KEY_VALUE_STATE = 3;
var DEFINE_KEY_STATE = 4;
var DEFINE_VALUE_STATE = 5;
var DEFINE_KEY_END_STATE = 6;
var DEFINE_VALUE_END_STATE = 7;
var DEFINE_BEGIN_VALUE_STATE = 8;

var UNEXPECTED_TOKEN_STATE = 10;
var UNEXPECTED_SCOPE_STATE = 11;

var DELIMITER_STATE = 14;
var ESCAPE_CHAR_STATE = 15;

var LET_BEGIN_STATE = 20;
var LET_KEY_STATE = 21;
var LET_KEY_ARGUMENTS_BEGIN_STATE = 22;
var LET_KEY_ARGUMENTS_STATE = 23;
var LET_KEY_ARGUMENTS_END_STATE = 24;
var LET_KEY_END_STATE = 25;
var LET_VALUE_BODY_BEGIN_STATE = 26;
var LET_VALUE_BODY_STATE = 27;
var LET_VALUE_BODY_END_STATE = 28;

var VAR_BEGIN_STATE = 40;
var VAR_KEY_VALUE_STATE = 41;
var VAR_KEY_STATE = 42;
var VAR_VALUE_STATE = 43;
var VAR_KEY_END_STATE = 44;
var VAR_VALUE_END_STATE = 45;
var VAR_BEGIN_VALUE_STATE = 46;

function tokenHintState (str, i, state) {
  if ('define' == str.substr(i-1, 6)) {
    return DEFINE_BEGIN_STATE;
  } else if ('let' == str.substr(i - 1, 3)) {
    return LET_BEGIN_STATE;
  } else if ('var' == str.substr(i - 1, 3)) {
    return VAR_BEGIN_STATE;
  }
  return state;
}

var __FILE__ = process.argv[2];

if (!__FILE__) {
  throw new Error("missing file");
}

var lineno = 1;
var col = 1;

function parse (str, scope) {
  // runtime
  var consts = {};
  var vars = {};
  var lets = {};

  var strBuffer = [];
  var currentLet = scope || null;
  var lastState = -1;
  var state = BEGIN_STATE;
  var needsQuote = false;
  var quotesNeeded = 0;
  var bracesNeeded = 0;
  var needsDelimiter = false;
  var needsBrace = false;
  var pendingInput = false;
  var lastToken = null;
  var parsed = {};
  var hint = null;
  var pending = null;
  var ch = null;
  var next = null;
  var prev = null;
  var buf = [];
  var i = 0;
  var key = null;
  var value = null;

  var isString = false;
  var isNumber = false;
  var isStruct = false;
  var isVar = false;
  var isBool = false;

  while (ch = str[i++]) {
    next = str[i];
    prev = str[i-2];

    switch (state) {
      case CLEAN_WHITE_SPACE_STATE:
        if (' ' == ch || '\t' == ch ||
            '\r' == ch || '\n' == ch) {
          strBuffer.push(ch);
        } else { i--; col--; state = hint; }
        break;

      case BEGIN_STATE:
        strBuffer = [];
        buf = [];
        state = tokenHintState(str, i, state);
        strBuffer.push(ch);
        buf.push(ch);
        break;

      case DEFINE_BEGIN_STATE:
        buf.push(ch);
        if ('define' == buf.join('')) {
          state = CLEAN_WHITE_SPACE_STATE;
          hint = DEFINE_KEY_STATE;
          pendingInput = true;
          buf = [];
        }
        break;

      case DEFINE_KEY_STATE:
        if (' ' == ch) {
          state = CLEAN_WHITE_SPACE_STATE;
          if (!buf.length) { hint = DEFINE_KEY_STATE; }
          else { hint = DEFINE_KEY_END_STATE; }
          break;
        } else if (!isascii(ch)) {
          lastState = state;
          state = UNEXPECTED_TOKEN_STATE;
          lastToken = ch;
          break;
        }
        buf.push(ch);
        break;

      case DEFINE_KEY_END_STATE:
        var key = buf.join('');
        buf = [];
        consts[key] = null;
        state = CLEAN_WHITE_SPACE_STATE;
        hint = DEFINE_BEGIN_VALUE_STATE;
        break;

      case DEFINE_BEGIN_VALUE_STATE:
        state = CLEAN_WHITE_SPACE_STATE;
        hint = DEFINE_VALUE_STATE;
        pendingInput = true;
        // check prev because of lack of '=' symbol
        if ('"' == prev) { needsQuote = true; }
        buf.push(ch);
        break;

      case DEFINE_VALUE_STATE:
        if ('"' == ch && needsQuote) {
          needsQuote = false;
        } else if ('"' == ch) {
          needsQuote = true;
        } else if (';' == ch && !needsQuote) {
          state = DEFINE_VALUE_END_STATE;
          i--; col--;
        } else {
          buf.push(ch);
        }
        break;

      case DEFINE_VALUE_END_STATE:
        consts[key] = buf.join('');
        key = null;
        buf = [];
        lastState = state;
        needsQuote = false;
        quotesNeeded = 0;
        state = CLEAN_WHITE_SPACE_STATE;
        hint = DELIMITER_STATE;
        i--; col--;
        break;

      case LET_BEGIN_STATE:
        buf.push(ch);
        strBuffer.push(ch);
        if ('let' == buf.join('')) {
          state = CLEAN_WHITE_SPACE_STATE;
          hint = LET_KEY_STATE;
          buf = [];
        }
        break;

      case LET_KEY_STATE:
        strBuffer.push(ch);
        if ('(' == ch) {
          state = CLEAN_WHITE_SPACE_STATE;
          hint = LET_KEY_ARGUMENTS_BEGIN_STATE;
          var name = buf.join('').trim().replace(/s+/g,'');
          currentLet = lets[name] = {};
          currentLet.name = name;
          currentLet.args = [];
          buf = [];
        } else {
          buf.push(ch);
        }
        break;

      case LET_KEY_ARGUMENTS_BEGIN_STATE:
        strBuffer.push(ch);
        if (')' == ch) {
          state = CLEAN_WHITE_SPACE_STATE;
          hint = LET_KEY_ARGUMENTS_END_STATE;
        } else if (!isascii(ch)) {
          lastState = state;
          state = UNEXPECTED_TOKEN_STATE;
        } else if (' ' == ch) {
          state = CLEAN_WHITE_SPACE_STATE;
          hint = LET_KEY_ARGUMENTS_STATE;
        } else {
          state = LET_KEY_ARGUMENTS_STATE;
          buf = [];
          i--; col--;
          strBuffer.pop();
        }
        break;

      case LET_KEY_ARGUMENTS_STATE:
        if (')' == ch) {
          strBuffer.push(ch);
          currentLet.args.push(buf.join(''));
          buf = [];
          state = CLEAN_WHITE_SPACE_STATE;
          hint = LET_KEY_ARGUMENTS_END_STATE;
        } else if (',' == ch) { // arg sep
          strBuffer.push(ch);
          currentLet.args.push(buf.join(''));
          buf = [];
          state = CLEAN_WHITE_SPACE_STATE;
          hint = LET_KEY_ARGUMENTS_STATE;
        } else if (!isascii(ch)) {
          lastState = state;
          state = UNEXPECTED_TOKEN_STATE;
        } else if (' ' == ch) {
          state = CLEAN_WHITE_SPACE_STATE;
          hint = LET_KEY_ARGUMENTS_STATE;
        } else {
          strBuffer.push(ch);
          buf.push(ch);
        }
        break;

      case LET_KEY_ARGUMENTS_END_STATE:
        state = CLEAN_WHITE_SPACE_STATE;
        hint = LET_VALUE_BODY_BEGIN_STATE;
        i--; col--;
        break;

      case LET_VALUE_BODY_BEGIN_STATE:
        strBuffer.push(ch);
        if ('{' == ch) {
          state = CLEAN_WHITE_SPACE_STATE;
          hint = LET_VALUE_BODY_STATE;
          needsBrace = true;
          bracesNeeded++;
        } else {
          lastState = state;
          state = UNEXPECTED_TOKEN_STATE;
        }
        break;

      case LET_VALUE_BODY_STATE:
        strBuffer.push(ch);
        if ('}' == ch) {
          if (1 == bracesNeeded) { // closing brace
            state = LET_VALUE_BODY_END_STATE;
            bracesNeeded = 0;
          } else {
            bracesNeeded--;
          }
        } else if ('{' == ch) {
          bracesNeeded++;
        } else {
          buf.push(ch);
        }
        break;

      case LET_VALUE_BODY_END_STATE:
        currentLet.body = buf.join('');
        currentLet.parsed = parse(currentLet.body, currentLet);
        currentLet.string = strBuffer.join('');
        i--; col--;
        state = CLEAN_WHITE_SPACE_STATE;
        hint = BEGIN_STATE;
        break;

      case VAR_BEGIN_STATE:
        buf.push(ch);
        if ('var' == buf.join('')) {
          state = CLEAN_WHITE_SPACE_STATE;
          hint = VAR_KEY_STATE;
          buf = [];
        }
        break;

      case VAR_KEY_STATE:
        // left hand assignment
        if ('=' == ch || ' ' == ch) {
          state = CLEAN_WHITE_SPACE_STATE;
          hint = VAR_KEY_END_STATE;
        } else {
          buf.push(ch);
        }
        break;

      case VAR_KEY_END_STATE:
        if (RESERVED_VAR_CHARS.indexOf(buf[0]) > -1) {
          lastState = state;
          state = UNEXPECTED_TOKEN_STATE;
        } else {
          key = buf.join('');
          buf = [];
          vars[key] = null;
          state = CLEAN_WHITE_SPACE_STATE;
          hint = VAR_BEGIN_VALUE_STATE;
        }
        break;

      case VAR_BEGIN_VALUE_STATE:
        state = CLEAN_WHITE_SPACE_STATE;
        hint = VAR_VALUE_STATE;
        pendingInput = true;
        if ('"' == ch) {
          needsQuote = true;
          isString = true;
        } else if (!isascii(ch) || RESERVED_VAR_CHARS.indexOf(ch) > -1) {
          lastState = state;
          state = UNEXPECTED_TOKEN_STATE;
          break;
        } else {
          buf.push(ch);
        }

        if (!isString && Number(ch) != Number(ch)) {
          isVar = true;
        } else if (Number(ch) == Number(ch)) {
          isNumber = true;
        } else if (!isString && !isNumber && !isVar && !isStruct) {
          lastState = state;
          state = UNEXPECTED_TOKEN_STATE;
        }
        break;

      case VAR_VALUE_STATE:
        if ('"' == ch && needsQuote) {
          needsQuote = false;
        } else if ('"' == ch) {
          needsQuote = true;
        } else if (';' == ch && !needsQuote) {
          state = VAR_VALUE_END_STATE;
          i--; col--;
        } else {
          buf.push(ch);
        }
        break;

      case VAR_VALUE_END_STATE:
        if (null === currentLet) {
          lastState = state;
          state = UNEXPECTED_SCOPE_STATE;
          break;
        } else {
          if ('true' == buf.join('') || 'false' == buf.join('')) {
            isBool = true;
          }

          value = buf.join('');

          if (isVar) {
            value = vars[value].value;
          }

          vars[key] = {
            value: value,
            isString: isString,
            isNumber: isNumber,
            isVar: isVar,
            isStruct: isStruct,
            isBool: isBool
          };

          state = CLEAN_WHITE_SPACE_STATE;
          hint = DELIMITER_STATE;
          i--; col--;
          needsQuote = false;
          quotesNeeded = 0;
          isString =
          isNumber =
          isBool =
          isStruct =
          isVar = false;
        }
        break;

      case DELIMITER_STATE:
        if (';' != ch) {
          throw new SyntaxError("Unexpected token `"+ ch +"'. Expecting token `;'");
        }
        pendingInput = false;
        state = CLEAN_WHITE_SPACE_STATE;
        hint = BEGIN_STATE;
        break;

      case UNEXPECTED_TOKEN_STATE:
        console.error("%s:%s:%s[s%d] >> '%s'",
                      __FILE__, ++lineno, --col,
                      lastState,
                      str.substr(i-col, i).trim());

        throw new SyntaxError("Unexpected token `"+ prev +"'");

      case UNEXPECTED_SCOPE_STATE:
        throw new Error("Invalid scope for `"+ key +"'");
    }

    if (state != UNEXPECTED_SCOPE_STATE &&
        state != UNEXPECTED_TOKEN_STATE) {
      if ('\n' == next || '\r' == next) {
        col = 1;
        lineno++;
      } else {
        col++;
      }

      if (('\n' == prev || '\r' == prev) && col != 1) {
        lineno--;
      }
    }

  }

  if (pendingInput) {
    throw new Error("Unexpected end of input '> "+ str.substr(i, i + 10) + "'...");
  }

  parsed.consts = consts;
  parsed.vars = vars;
  parsed.lets = lets;

  return parsed;
}


try {
  var val = parse(String(require('fs').readFileSync(process.argv[2])));
  console.log(val.lets);
} catch (e) {
  console.error("%s: %s", e.name, e.message);
}
