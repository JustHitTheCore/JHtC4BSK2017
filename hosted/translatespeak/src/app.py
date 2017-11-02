# <!--
import os
import shlex
import subprocess
import logging
from uuid import uuid4

from flask import Flask, request, redirect
from googletrans import Translator


logging.basicConfig(level=logging.DEBUG)

app = Flask(__name__)

flag_1 = os.environ['JHtC4BSK_FIRST_FLAG']

base = """
<form action="/translate">
  Translate string:<br>
  <input type="text" name="translate" value=""><br/>
  Source lang:<br>
  <input type="text" name="src" value="pl"><br/>
  Dest lang:<br>
  <input type="text" name="dst" value="en"><br/>
  <input type="submit" value="Submit">
</form>
"""

hear = """
"""

@app.route('/')
def root():
    return base

TMP_PATH = '/tmp'

@app.route('/translate')
def translate():
    string = request.args.get('translate')
    dst = request.args.get('dst', 'en')
    src = request.args.get('src', 'pl')

    if string:
        string = string[:100]

        tr = Translator().translate(string, dest=dst, src=src)

        fname = os.path.join(TMP_PATH, str(uuid4()))
        try:
            cmd = 'espeak --stdout {}'.format(shlex.quote(string))
            cmd += ' > {0}'
            cmd = cmd.format("'" + fname + "'")
            logging.info('Trying to invoke %s' % cmd)
            subprocess.check_output(cmd, shell=True, env={})
        except Exception as e:
            fname = None
            raise e
        
        render = base + '<br><br>Translated %s to %s' % (tr.origin, tr.text)
        
        if fname:
            render += '<br><br>Download espeak <a href="%s">here</a>' % fname
            render += """
<br>
<script>
var audio = new Audio('%s');
audio.play();
</script>
""" % fname

        return render

    return ''

@app.route(TMP_PATH + '/<filename>')
def tmp(filename):
    if 'flag' in filename:  # /tmp/flag_2, /tmp/flag_3
        return 'lol no'

    with open(os.path.join(TMP_PATH, filename), 'rb') as f:
        return f.read()

# fake server
@app.route('/robots.txt')
def robots():
    return cachedfile(os.path.realpath('robots.txt'))

@app.route('/backup')
def backup():
    if request.headers.get('User-Agent') != 'magic':
        return redirect('https://www.youtube.com/watch?v=dQw4w9WgXcQ', code=418)

    filename = request.args.get('fname', os.path.realpath(__file__))

    if 'flag_3' in filename:
        return 'lol no'

    return base + cachedfile(filename)


cache = {}
def cachedfile(fname):
    print("Requesting ", fname)
    if fname not in cache:
        try:
            with open(fname) as f:
                print('Saving file %s in cache' % fname)
                cache[fname] = f.read()
        except FileNotFoundError:
            return '<!-- File not found, sorry --!>'

    return cache[fname]

if __name__ == '__main__':
    app.run(debug=True)

# --!>
