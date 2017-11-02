Rails.application.routes.draw do
  root 'safe_login#index'
  match "/login" => "safe_login#login", :via => :post
end
