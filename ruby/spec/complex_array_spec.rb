# encoding: utf-8

require File.join(File.expand_path(File.dirname(__FILE__)), '../lib/eigenbrötler')

describe ComplexArray, "construction" do
  it "should have 0 height and 0 width if constructed with no arguments" do
    m = ComplexArray.new
    m.height.should eq 0
    m.width.should eq 0
  end

  w = 128
  h = 256
  it "should have height #{h} and width #{w} if constructed with arguments (#{w}, #{h})" do
    m = ComplexArray.new(w, h)
    m.height.should eq h
    m.width.should eq w
  end
  it "should contain zeros if constructed with arguments (#{w}, #{h})" do
    m = ComplexArray.new(w, h)
    w.times do |i|
      h.times do |j|
        m[i, j].real.should be_within(1.0e-08).of(0.0)
        m[i, j].imag.should be_within(1.0e-08).of(0.0)
      end
    end
  end
end
