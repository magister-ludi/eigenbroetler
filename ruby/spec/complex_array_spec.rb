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
  it "should fail and set an error if trying to read a non-existent file" do
    fname = "a non-existent file"
    m = ComplexArray.new(fname)
    m.valid?.should eq false
    m.err.should eq "\"#{fname}\" doesn't appear to exist!"
  end
  it "should load a bitmapped image successfully" do
    fname = File.join(File.dirname(__FILE__), 'about_icon.ppm')
    m = ComplexArray.new(fname)
    puts m.err unless m.valid?
    m.valid?.should eq true
    m.width.should eq 110
    m.height.should eq 110
  end
  it "should load a FITS image successfully" do
    fname = File.join(File.dirname(__FILE__), '5eb4572e81.fit')
    m = ComplexArray.new(fname)
    puts m.err unless m.valid?
    m.valid?.should eq true
    m.width.should eq 240
    m.height.should eq 180
  end
end
