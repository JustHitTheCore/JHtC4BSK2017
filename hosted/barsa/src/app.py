from flask import Flask
from flask import render_template
from flask import make_response
from flask import request
from flask import redirect
from flask import url_for
from flask import send_from_directory
from flask_sqlalchemy import SQLAlchemy
from flask_bcrypt import Bcrypt

from CryptoLib import SecureKey
import sys, re, os
from random import randint
import string


#---CONFIG---
app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:////tmp/test.db'
db = SQLAlchemy(app)
bcrypt = Bcrypt(app)
FLAG = '/images/fc96f22d5230d38d0a81d2efc86ef5c051ecb811b4b601e2bb18909f0064fb92.gif'

enc_key = SecureKey()
sig_key = SecureKey()
if os.path.isfile('encryption_key.sec') and os.path.isfile('signing_key.sec'):
    enc_key.importKey('encryption_key.sec')
    sig_key.importKey('signing_key.sec')
    # print "Keys imported"
else:
    print "Keys not found"
    sys.exit(1)

#---MODEL---
class User(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(30), unique=True)
    password = db.Column(db.String(100), unique=True)
    flag = db.Column(db.String(120))

    def __init__(self, username, password, flag):
        self.username = username
        self.password = password
        self.flag = flag

    def __repr__(self):
        return '<User %r>' % self.username


#---CRYPTO FUNCTIONS---
def encrypt(plain):
    encrypted = enc_key.encrypt(plain)
    signed = sig_key.sign(encrypted)
    cip = encrypted.encode('hex') + '--' + signed.encode('hex')
    return cip

def decrypt(cip):
    try:
        encrypted, signature = map(lambda x: x.decode('hex'), cip.split('--'))
        if sig_key.verify(encrypted, signature) == False:
            return False
        plain = enc_key.decrypt(encrypted)
        return plain
    except:
        return False

def get_user(auth):
    if auth:
        username = decrypt(auth)
        if username != False:
            return User.query.filter_by(username=username).first()
    return None


#---MAIN APP---
@app.route('/css/<path:filename>')
def send_css(filename):
    root_dir = os.getcwd()
    return send_from_directory(os.path.join(root_dir, 'static', 'css'), filename)

@app.route('/images/<path:filename>')
def send_images(filename):
    if '/images/' + filename == FLAG:
        user = get_user(request.cookies.get('auth'))
        if user is None or user.username != 'admin':
            return redirect(url_for('index'))
    root_dir = os.getcwd()
    return send_from_directory(os.path.join(root_dir, 'static', 'images'), filename)

@app.route('/robots.txt')
def send_robots():
    root_dir = os.getcwd()
    return send_from_directory(os.path.join(root_dir, 'static'), 'robots.txt')

@app.route('/do_not_look_at_me.zip')
def send_do_not():
    root_dir = os.getcwd()
    return send_from_directory(os.path.join(root_dir, 'static'), 'do_not_look_at_me.zip')

@app.route("/flag", methods=['GET'])
def flag():
    fake_flag = User.query.filter_by(username='flag').first()
    return render_template('index.html', user=fake_flag)

@app.route("/", methods=['GET'])
def index():
    user = get_user(request.cookies.get('auth'))
    return render_template('index.html', user=user)

@app.route("/register", methods=['GET', 'POST'])
def register():
    if get_user(request.cookies.get('auth')):
        return redirect(url_for('index'))
    if request.method == 'POST' and 'username' in request.form and 'password' in request.form:
        username = request.form['username']
        password = request.form['password']
        error = ''
        if len(password) < 5:
            error = "Password length must be greater than 5 chars"
        if len(username) < 5:
            error = "Username length must be greater than 5 chars"
        if len(username) >= 25:
            error = "Username length must be lower than 25 chars"
        if re.search("[^a-zA-Z0-9_]", username) != None:
            error = "Username in [a-zA-Z0-9_]"
        if error:
            return render_template('register.html', error=error)
        if User.query.filter_by(username=username).first() != None:
            return render_template('register.html', error='User already exists')

        user_flag = '/images/mp'+str(randint(1,7))+'.gif'
        password = bcrypt.generate_password_hash(password)
        user = User(username, password, user_flag)
        db.session.add(user)
        db.session.commit()

        auth = encrypt(username)
        resp = make_response(render_template('index.html', user=user, flash='Successfully registered'))
        resp.set_cookie('auth', auth)
        return resp
    else:
        return render_template('register.html')

@app.route("/login", methods=['GET', 'POST'])
def login():
    if get_user(request.cookies.get('auth')):
        return redirect(url_for('index'))
    if request.method == 'POST' and 'username' in request.form and 'password' in request.form:
        username = request.form['username']
        password = request.form['password']

        user = User.query.filter_by(username=username).first()
        if user == None or not bcrypt.check_password_hash(user.password, password):
            return render_template('login.html', error='Invalid username/password')

        auth = encrypt(username)
        resp = make_response(render_template('index.html', user=user, flash='Successfully logged in'))
        resp.set_cookie('auth', auth)
        return resp
    else:
        return render_template('login.html')

@app.route("/logout", methods=['GET'])
def logout():
    if request.cookies.get('auth'):
        resp = make_response(render_template('index.html', flash='You were successfully logged out'))
        resp.set_cookie('auth', '', expires=0)
        return resp
    else:
        return redirect(url_for('index'))


if __name__ == "__main__":
    db.drop_all()
    db.create_all()

    alphabet = string.printable[:-38]
    password = ''.join([alphabet[randint(0, len(alphabet)-1)] for _ in range(40)])
    print "admin password:", repr(password)
    password = bcrypt.generate_password_hash(password)
    admin = User('admin', password, FLAG)
    db.session.add(admin)

    password = 'flag'
    password = bcrypt.generate_password_hash(password)
    fake_flag = User('flag', password, '/images/flag.gif')
    db.session.add(fake_flag)

    db.session.commit()

    enc_key = SecureKey()
    sig_key = SecureKey()
    if os.path.isfile('encryption_key.sec') and os.path.isfile('signing_key.sec'):
        enc_key.importKey('encryption_key.sec')
        sig_key.importKey('signing_key.sec')
        print "Keys imported"
    else:
        enc_key.generate()
        enc_key.exportKey('encryption_key.sec')
        sig_key.generate()
        sig_key.exportKey('signing_key.sec')
        print "Keys generated"

    app.run(host = '0.0.0.0')