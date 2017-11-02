class SafeLoginController < ApplicationController
  def index
  end
  
  def login
    input = params[:password]
    new_input = ""
    for i in 0..input.length - 1
      new_input += let(input[i].chr)
    end
    flash['time'] = benchmark(new_input, ENV['PASS'])
    if @result
      render json: 'Nice one! Flag is the password!'
    else
      render 'safe_login/index'
    end
  end
 private

 def benchmark(input, password)
   counter = 0
   if input.length != password.length || input[0] == '0'
     return rand(0.2..0.3).round(3)
    else
      for i in 0..input.length
        if input[i] == password[i]
          counter += 1
        else
          return (counter / 1000.0).round(3)
        end
      end
    end
    @result = true
  end
end


